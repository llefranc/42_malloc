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
#include <string.h>
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
*/
int lmmap_get_size(struct mmaphdr *ptr)
{
	int nb = 0;

	while (ptr) {
		ptr = ptr->next;
		++nb;
	}
	return nb;
}

/**
 * Do a mmap and init at the beginning of the mmapped area the struct mmaphdr.
 *
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
	return tmp;
}

/**
 * Do a mmap, init at the beginning of the mmapped area the struct mmaphdr,
 * and add this new mmaped memory at the end of a mmap linked list represented
 * by ptr.
 *
 * Return: A pointer to the new mmaped area (e.g. the new elem at the end of
 *         the mmap list) which can be cast in struct mmaphdr pointer, or
 *         NULL if an error occured.
*/
void * lmmap_push_back(struct mmaphdr *ptr, size_t size)
{
	struct mmaphdr *tmp;

	if (ptr == NULL)
		return NULL;
	size = rnd_to_page_size(size);
	tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		   MAP_ANONYMOUS, -1, 0);
	if (tmp == NULL)
		return NULL;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = tmp;
	tmp->prev = ptr;
	tmp->next = NULL;
	tmp->size = size;
	tmp->nb_alloc = 0;
	return tmp;
}

/**
 * Remove an elem from mmap list and munmap its memory.
 *
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
*/
void lmmap_print_elem(struct mmaphdr *elem)
{
	printf("------ mmap (%p) ------\n", elem);
	printf("prev mmap addr = %p\n", elem->prev);
	printf("next mmap addr = %p\n", elem->next);
	printf("size mmap = %zu\n", elem->size);
	printf("nb alloc = %d\n", elem->nb_alloc);
}

/**
 * Print the infos for each elem of a mmap linked list.
*/
void lmmap_print_all(struct mmaphdr *ptr)
{
	while (ptr) {
		lmmap_print_elem(ptr);
		ptr = ptr->next;
	}
}