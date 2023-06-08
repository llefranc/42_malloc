/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/08 17:45:56 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

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

static struct chkhdr * alloc_tiny(size_t size)
{
	struct mmaphdr *new_m;
	struct chkhdr *chk;

	size = chk_size_16align(size);
	if ((chk = lmmap_bestfit(bins.tiny, size)) == NULL) {
		if ((new_m = create_bin(&bins.tiny, TINY_MMAP_SIZE)) == NULL)
			return NULL;
		chk = new_m->first_free.next_free;
	}
	return chk_alloc(chk, size);
}

static struct chkhdr * alloc_small(size_t size)
{
	struct mmaphdr *new_m;
	struct chkhdr *chk;

	size = chk_size_16align(size);
	if ((chk = lmmap_bestfit(bins.small, size)) == NULL) {
		if ((new_m = create_bin(&bins.small, SMALL_MMAP_SIZE)) == NULL)
			return NULL;
		chk = new_m->first_free.next_free;
	}
	return chk_alloc(chk, size);
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
	struct chkhdr *new_c;
	struct mmaphdr *new_m;

	if ((new_m = create_bin(&bins.large, mmap_size)) == NULL)
		return NULL;
	/*
	 * Large allocated chunk will fit the whole mmap size because this
	 * space is lost anyway.
	 */
	size = new_m->size - (sizeof(struct mmaphdr) + BNDARY_TAG_SIZE * 2);
	if ((new_c = chk_alloc(new_m->first_chk, size)) == NULL)
		return NULL;
	new_m->nb_alloc++;
	return new_c;
}

void *ft_malloc(size_t size)
{
	struct chkhdr *new_alloc = NULL;

	if (!size)
		return NULL;
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
	return (void *)(((uint8_t *)new_alloc) + BNDARY_TAG_SIZE);
}