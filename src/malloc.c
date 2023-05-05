/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/04/21 17:17:25 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#define BIN_MAX_ALLOC (100)
#define BOUNDARY_TAG_SIZE (8)

#define TINY_MAX_CHUNK_SIZE (256)
#define TINY_MAX_ALLOC_SIZE (TINY_MAX_CHUNK_SIZE - 2 * BOUNDARY_TAG_SIZE)

#define SMALL_MAX_CHUNK_SIZE (1024)
#define SMALL_MAX_ALLOC_SIZE (SMALL_MAX_CHUNK_SIZE - 2 * BOUNDARY_TAG_SIZE)


/**
 * Contain pointer to a mapping in the virtual address space of the calling
 * proces and the mapping size. This memory is used to store allocated memory.
*/
// struct bin {   >> useless car size ne marche pas quand on chain les maps
// 	void *ptr;
// 	size_t size;
// };

/**
 * Contain the differents bins for tiny, small and large malloc, which are a
 * linked list of mmaps.
 *
 * @tiny: x mmap containing x tiny chunks of memory.
 * @small: x mmap containing x small chunks of memory.
 * @large: x mmap containing each one one large allocation.
*/
struct s_bins {
	void *tiny;
	void *small;
	void *large;
};

// static struct s_bins bins;
// static int page_size;

/**
 * Return a size rounded to a multiple of page_size.
*/
static inline size_t rnd_to_page_size(size_t size)
{
	int page_size = getpagesize();
	size_t tmp = size % page_size;

	if (tmp > 0)
		tmp = page_size - tmp;

	printf("rnd page size = %zu\n", size + tmp);
	return size + tmp;
}

/**
 * Init bin's address and bin' size by mmaping size bytes rounded to page size.
*/
// static int init_bin(struct bin *b, size_t size)
// {
// 	b->size = rnd_to_page_size(size);
// 	b->ptr = mmap(NULL, b->size, PROT_READ | PROT_WRITE, MAP_PRIVATE |
// 		      MAP_ANONYMOUS, -1, 0);
// 	if (b->ptr == MAP_FAILED)
// 		return -1;
// 	return 0;
// }


/**
 * Init page_size and the tiny, small and large bins by doing a mmap for each
 * of them.
*/
// int init(void)
// {
// 	static _Bool is_init = 0;

// 	if (is_init)
// 		return 0;

// 	page_size = getpagesize();
// 	if (init_bin(&bins.tiny, TINY_MAX_CHUNK_SIZE * BIN_MAX_ALLOC) == -1 ||
// 	    init_bin(&bins.small, SMALL_MAX_CHUNK_SIZE * BIN_MAX_ALLOC) == -1 ||
// 	    init_bin(&bins.large, page_size) == -1)
// 		return -1;

// 	is_init = 1;
// 	printf("init done\n");
// 	printf("pagesize = %d\n", page_size);
// 	printf("tiny addr = %p and size = %zu\n", bins.tiny.ptr, bins.tiny.size);
// 	printf("small addr = %p and size = %zu\n", bins.small.ptr, bins.small.size);
// 	printf("large addr = %p and size = %zu\n", bins.large.ptr, bins.large.size);
// 	return 0;
// }

// void show_alloc_mem(void)
// {
// 	printf("TINY : %p\n", bins.tiny.ptr);
// 	printf("TINY : %p\n", bins.large.ptr);
// 	printf("TINY : %p\n", bins.large.ptr);
// 	printf("TINY : %p\n", bins.large.ptr);
// }

void *ft_malloc(size_t size)
{
	// printf("my_ft_malloc\n");
	// if (init() == -1) {
	// 	fprintf(stderr, "init error\n");
	// 	errno = ENOMEM;
	// 	return NULL;
	// }

	// if (clear_bin(&bins.tiny) == -1) {
	// 	printf("err cleaning tiny\n");
	// }
	// if (clear_bin(&bins.small) == -1) {
	// 	printf("err cleaning small\n");
	// }
	// if (clear_bin(&bins.large) == -1) {
	// 	printf("err cleaning large\n");
	// }
	(void)size;
	return NULL;
}