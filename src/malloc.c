/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/01 19:02:25 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "bins.h"
#include "lmmap.h"
#include "chunk.h"

#include <errno.h>

struct s_bins bins = { .tiny = NULL, .small = NULL, .large = NULL };

static struct chkhdr * alloc_tiny(size_t size)
{
	(void)size;
	return NULL;
}

static struct chkhdr * alloc_small(size_t size)
{
	(void)size;
	return NULL;
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

	if (bins.large == NULL) {
		new_m = lmmap_new(&bins.large, mmap_size);
	} else {
		new_m = lmmap_push_back(bins.large, mmap_size);
	}
	if (!new_m)
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