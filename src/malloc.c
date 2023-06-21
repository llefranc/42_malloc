/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 11:16:14 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "mutex.h"
#include "bins.h"
#include "lmmap.h"
#include "chunk.h"

#include <errno.h>

struct s_bins bins = { .tiny = NULL, .small = NULL, .large = NULL };

static inline struct mmaphdr * create_bin(struct mmaphdr **head,
					  size_t mmap_size)
{
	if (*head == NULL)
		return lmmap_new(head, mmap_size);
	return lmmap_push_back(*head, mmap_size);
}

/**
 * Allocate a tiny area of memory by finding the best free chunk avaible in the
 * tiny bins. If this is the first tiny malloc that the process is doing or no
 * free chunk are available, then create a new tiny bin and do the allocation
 * inside.
*/
static struct chkhdr * alloc_tiny(size_t size)
{
	struct mmaphdr *bin;
	struct chkhdr *chk;

	size = chk_size_16align(size);
	chk = lmmap_bestfit(bins.tiny, size);
	if (chk != NULL) {
		if ((bin = lmmap_get_elem(bins.tiny, chk)) == NULL)
			return NULL;
	} else {
		if ((bin = create_bin(&bins.tiny, TINY_MMAP_SIZE)) == NULL)
			return NULL;
		chk = bin->first_free.next_free;
	}
	if ((chk = chk_alloc(chk, size)) == NULL)
		return NULL;
	bin->nb_alloc++;
	return chk;
}

/**
 * Allocate a small area of memory by finding the best free chunk avaible in the
 * small bins. If this is the first small malloc that the process is doing or no
 * free chunk are available, then create a new small bin and do the allocation
 * inside.
*/
static struct chkhdr * alloc_small(size_t size)
{
	struct mmaphdr *bin;
	struct chkhdr *chk;

	size = chk_size_16align(size);
	chk = lmmap_bestfit(bins.small, size);
	if (chk != NULL) {
		if ((bin = lmmap_get_elem(bins.small, chk)) == NULL)
			return NULL;
	} else {
		if ((bin = create_bin(&bins.small, SMALL_MMAP_SIZE)) == NULL)
			return NULL;
		chk = bin->first_free.next_free;
	}
	if ((chk = chk_alloc(chk, size)) == NULL)
		return NULL;
	bin->nb_alloc++;
	return chk;
}

/**
 * Allocate a large area of memory, consisting of 1 mmap area with only one
 * chunk inside filling all the space.
*/
static struct chkhdr * alloc_large(size_t size)
{
	/*
	 * One boundary tag is already inside struct mmaphdr, but we need to
	 * count the last byte of the mmap area not used because of 16 align
	 * chunks: the last footer will be align on 16, and so there is a last
	 * unused byte. So we do BNDARY_TAG_SIZE * 2 to count it.
	 */
	size_t mmap_size = size + sizeof(struct mmaphdr) + BNDARY_TAG_SIZE * 2;
	struct mmaphdr *bin;
	struct chkhdr *chk;

	if ((bin = create_bin(&bins.large, mmap_size)) == NULL)
		return NULL;
	/*
	 * Large allocated chunk will fit the whole mmap size because this
	 * space is lost anyway.
	 */
	size = bin->size - (sizeof(struct mmaphdr) + BNDARY_TAG_SIZE * 2);
	if ((chk = chk_alloc(bin->first_chk, size)) == NULL)
		return NULL;
	bin->nb_alloc++;
	return chk;
}

/**
 * Allocate on the heap memory the requested size.
 *
 * @size: The size to allocate.
 * Return: A pointer to the newly allocated area. NULL with errno set to ENOMEM
 *         if there is not enough memory.
*/
void *ft_malloc(size_t size)
{
	struct chkhdr *new_alloc = NULL;

	mutex_lock();
	if (!size) {
		size = 1;
	}
	if (size <= TINY_MAX_ALLOC_SIZE) {
		new_alloc = alloc_tiny(size);
	} else if (size <= SMALL_MAX_ALLOC_SIZE) {
		new_alloc = alloc_small(size);
	} else {
		new_alloc = alloc_large(size);
	}
	if (new_alloc == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	mutex_unlock();
	return (void *)(((uint8_t *)new_alloc) + BNDARY_TAG_SIZE);
}