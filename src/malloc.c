/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/23 14:24:52 by llefranc         ###   ########.fr       */
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

static struct chkhdr * alloc_large(size_t size)
{
	struct chkhdr *new_c;
	struct mmaphdr *new_m;

	if (bins.large == NULL) {
		new_m = lmmap_new(&bins.large, size);
	} else {
		new_m = lmmap_push_back(bins.large, size);
	}
	if (!new_m)
		return NULL;
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