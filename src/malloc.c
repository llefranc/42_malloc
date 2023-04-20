/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/04/20 17:57:09 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define BIN_MAX_ALLOC 100
#define TINY_MAX_BYTES 16
#define SMALL_MAX_BYTES 128

struct bin {
	void *ptr;
	size_t size;
};

struct all_bins {
	struct bin tiny;
	struct bin small;
	struct bin large; // a revoir car double tableau
};

static struct all_bins bins;
static int page_size;

static inline size_t rnd_to_page_size(size_t size)
{
	size_t tmp = size % page_size;

	if (tmp > 0)
		tmp = page_size - tmp;

	printf("rnd page size = %zu\n", size + tmp);
	return size + tmp;
}

static inline void *my_mmap(size_t size)
{
	return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		    MAP_ANONYMOUS, -1, 0);
}

void init(void)
{
	static _Bool is_init = 0;

	if (is_init)
		return;

	page_size = getpagesize();

	bins.tiny.size = rnd_to_page_size(TINY_MAX_BYTES * BIN_MAX_ALLOC);
	bins.small.size = rnd_to_page_size(SMALL_MAX_BYTES * BIN_MAX_ALLOC);
	bins.large.size = page_size;

	bins.tiny.ptr = my_mmap(bins.tiny.size);
	bins.small.ptr = my_mmap(bins.small.size);
	bins.large.ptr = my_mmap(bins.large.size);
	is_init = 1;
	printf("init done\n");
	printf("pagesize = %d\n", page_size);
	printf("tiny addr = %p and size = %zu\n", bins.tiny.ptr, bins.tiny.size);
	printf("tiny addr = %p and size = %zu\n", bins.small.ptr, bins.small.size);
	printf("tiny addr = %p and size = %zu\n", bins.large.ptr, bins.large.size);
}

void *malloc(size_t size)
{
	init();
	(void)size;
	return NULL;
}