/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/11 11:31:11 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "bins.h"
#include "lmmap.h"
#include "chunk.h"

#include <errno.h>

static void * alloc_tiny(size_t size)
{
	(void)size;
	return NULL;
}

static void * alloc_small(size_t size)
{
	(void)size;
	return NULL;
}

static void * alloc_large(size_t size)
{
	(void)size;
	return NULL;
}

void *ft_malloc(size_t size)
{
	void *new_alloc = NULL;

	if (!size)
		return NULL;
	if (size < TINY_MAX_CHUNK_SIZE)
		new_alloc = alloc_tiny(size);
	else if (size < SMALL_MAX_CHUNK_SIZE)
		new_alloc = alloc_small(size);
	else
		new_alloc = alloc_large(size);
	return new_alloc;
}