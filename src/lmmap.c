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
int lmmap_get_size(void *ptr)
{
	int nb = 0;
	struct mmap_info *elem = ptr;

	while (elem) {
		elem = elem->next;
		++nb;
	}
	return nb;
}

/**
 * Do a mmap and init at the beginning of the mmapped area the struct mmap_info.
 *
 * Return: A pointer to the mmaped area which can be cast in struct mmap_info
 *         pointer, or NULL if an error occured.
*/
void * lmmap_new(size_t size)
{
	struct mmap_info *tmp;

	size = rnd_to_page_size(size);
	tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		   MAP_ANONYMOUS, -1, 0);
	if (tmp == NULL)
		return NULL;
	tmp->prev = NULL;
	tmp->next = NULL;
	tmp->size = size;
	return tmp;
}

/**
 * Do a mmap, init at the beginning of the mmapped area the struct mmap_info,
 * and add this new mmaped memory at the end of a mmap linked list represented
 * by ptr.
 *
 * Return: A pointer to the new mmaped area (e.g. the new elem at the end of
 *         the mmap list) which can be cast in struct mmap_info pointer, or
 *         NULL if an error occured.
*/
void * lmmap_push_back(void *ptr, size_t size)
{
	struct mmap_info *elem = ptr;
	struct mmap_info *tmp;

	if (ptr == NULL)
		return NULL;
	size = rnd_to_page_size(size);
	tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		   MAP_ANONYMOUS, -1, 0);
	if (tmp == NULL)
		return NULL;
	while (elem->next)
		elem = elem->next;
	elem->next = tmp;
	tmp->prev = elem;
	tmp->next = NULL;
	tmp->size = size;
	return tmp;
}

/**
 * Remove an elem from mmap list and munmap its memory.
 *
 * Return: 0 on success, -1 in case of error.
*/
int lmmap_rm_elem(void *ptr)
{
	struct mmap_info *elem = ptr;
	struct mmap_info *tmp;

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
 * Print infos stored at the beginning of a mmap.
*/
void mmap_print_info(struct mmap_info *ptr)
{
	printf("------ mmap (%p) ------\n", ptr);
	printf("prev mmap addr = %p\n", ptr->prev);
	printf("next mmap addr = %p\n", ptr->next);
	printf("size mmap = %zu\n", ptr->size);
}

/**
 * Print the infos for each elem of a mmap linked list.
*/
void lmmap_print(struct mmap_info *ptr)
{
	while (ptr) {
		mmap_print_info(ptr);
		ptr = ptr->next;
	}
}