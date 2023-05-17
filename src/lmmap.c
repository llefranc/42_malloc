/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lmmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:04 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/05 18:14:01 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lmmap.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>

/**
 * Return a size rounded to a multiple of page_size.
*/
static size_t rnd_to_page_size(size_t size)
{
	int page_size = getpagesize();
	size_t tmp = size % page_size;

	if (tmp > 0)
		tmp = page_size - tmp;

	printf("rnd page size = %zu\n", size + tmp);
	return size + tmp;
}

/**
 * Return the number of elements in a list of mmaps.
 *
 * @head: Head of the mmaps linked list.
*/
int lmmap_get_size(struct mmaphdr *head)
{
	int nb = 0;

	while (head) {
		head = head->next;
		++nb;
	}
	return nb;
}

/**
 * Init the first chunk starting from the last member of struct mmaphdr, so
 * chunk headers can be aligned on 8 and chunks body aligned on 16.
 * The first chunk will be considered as free and will cover the totality of
 * the mmap area. The first allocation will be done inside this chunk.
 *
 * >>> Dump of beginning of mmap area after init.
 * >>> 0x0020: the header used only to access the first free chunk (3 qwords,
 *             only next_free member of struct chkhdr is used (0x0030)).
 * >>> 0x0038: Start of the first chunk, which is free and contain the rest of
 *             the mmap area after init.
 * 0x0000: 00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00
 * 0x0010: 00 10 00 00 00 00 00 00   00 00 00 00 00 00 00 00
 * 0x0020: 00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00
 * 0x0030: 38 20 fa 6d 97 7f 00 00   b8 0f 00 00 00 00 00 00
 * 0x0040: 20 20 fa 6d 97 7f 00 00   00 00 00 00 00 00 00 00
*/
static void init_first_free_chk(struct mmaphdr *m)
{
	/*
	 * Substracting only 1 boundary tag size (e.g. size of footer) because
	 * size of header is already contained in sizeof(struct mmaphdr).
	 */
	size_t chk_size = m->size - sizeof(*m) - BNDARY_TAG_SIZE;
	struct chkhdr *hdr = (struct chkhdr *)&m->first_chk;
	struct chkftr *ftr;

	/* Init first chunk (one big free chunk) */
	hdr->is_alloc = 0;
	hdr->size = chk_size;
	hdr->prev_free = &m->first_free;
	hdr->next_free = NULL;
	ftr = chk_htof(hdr);
	ftr->is_alloc = 0;
	ftr->size = chk_size;

	/* Point the first chunk which is free after init. */
	m->first_free.next_free = hdr;
	m->first_free.prev_free = NULL;
}

/**
 * Do a mmap and init at the beginning of the mmapped area the struct mmaphdr.
 *
 * @size: The requested size for the mmap syscall. Size will be rounded up to
 *        a multiple of page_size.
 * Return: A pointer to the mmaped area which can be cast in struct mmaphdr
 *         pointer, or NULL if an error occured.
*/
void * lmmap_new(size_t size)
{
	struct mmaphdr *tmp;

	size = rnd_to_page_size(size);
	tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		   MAP_ANONYMOUS, -1, 0);
	if (tmp == NULL)
		return NULL;
	tmp->prev = NULL;
	tmp->next = NULL;
	tmp->size = size;
	tmp->nb_alloc = 0;
	init_first_free_chk(tmp);
	return tmp;
}

/**
 * Do a mmap, init at the beginning of the mmapped area the struct mmaphdr,
 * and add this new mmaped memory at the end of a mmap linked list represented
 * by head.
 *
 * @head: Head of the mmaps linked list.
 * @size: The requested size for the mmap syscall. Size will be rounded up to
 *        a multiple of page_size.
 * Return: A pointer to the new mmaped area (e.g. the new elem at the end of
 *         the mmap list) which can be cast in struct mmaphdr pointer, or
 *         NULL if an error occured.
*/
void * lmmap_push_back(struct mmaphdr *head, size_t size)
{
	struct mmaphdr *tmp;

	if (head == NULL)
		return NULL;
	size = rnd_to_page_size(size);
	tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		   MAP_ANONYMOUS, -1, 0);
	if (tmp == NULL)
		return NULL;
	while (head->next)
		head = head->next;
	head->next = tmp;
	tmp->prev = head;
	tmp->next = NULL;
	tmp->size = size;
	tmp->nb_alloc = 0;
	init_first_free_chk(tmp);
	return tmp;
}

/**
 * Remove an elem from mmap list and munmap its memory.
 *
 * @elem: The mmap elem to remove.
 * Return: 0 on success, -1 in case of error.
*/
int lmmap_rm_elem(struct mmaphdr *elem)
{
	struct mmaphdr *tmp;

	if (elem->prev) {
		tmp = elem->prev;
		tmp->next = elem->next;
	}
	if (elem->next) {
		tmp = elem->next;
		tmp->prev = elem->prev;
	}
	return munmap(elem, elem->size);
}

/**
 * Print infos of an elem from a linked list of mmaped area, using the struct
 * mmaphdr stored at the beginning of the mmap area.
 *
 * @elem: The mmap elem to print.
*/
void lmmap_print_elem(struct mmaphdr *elem)
{
	printf("------ mmap (%p) ------\n", elem);
	printf("prev mmap addr = %p\n", elem->prev);
	printf("next mmap addr = %p\n", elem->next);
	printf("size mmap = %zu\n", elem->size);
	printf("nb alloc = %zu\n", elem->nb_alloc);
	printf("-----------------------------------\n");
}

/**
 * Print the infos for each elem of a mmap linked list.
 *
 * @head: Head of the mmaps linked list.
*/
void lmmap_print_all(struct mmaphdr *head)
{
	while (head) {
		lmmap_print_elem(head);
		head = head->next;
	}
}