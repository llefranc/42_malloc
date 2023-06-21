/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_show_alloc_mem.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 21:10:43 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 11:31:02 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"

#include "../src/bins.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_show_alloc_mem_no_alloc(void)
{
	show_alloc_mem();
}

void test_show_alloc_mem_x_malloc(size_t nb_alloc, size_t start_alloc_size)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST show_alloc_mem_x_malloc <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	size_t alloc_size = start_alloc_size;

	for (size_t i = 0; i < nb_alloc; ++i) {
		printf("allocating size %zu\n", alloc_size);
		assert((ptr = ft_malloc(alloc_size)) != NULL);
		memset(ptr, letter++, alloc_size);
		alloc_size *= 2;
	}
	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST show_alloc_mem_x_malloc <<<<<<<<<\n");
}

void test_show_alloc_mem_empty_bin_in_the_middle(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST show_alloc_mem_empty_bin_in_the_middle <<<<<<<<<\n");
	struct mmaphdr *last;

	assert(ft_malloc(1) != NULL);
	assert(lmmap_push_back(bins.tiny, TINY_MMAP_SIZE) != NULL);
	assert((last = lmmap_push_back(bins.tiny, TINY_MMAP_SIZE)) != NULL);
	assert(chk_alloc(last->first_free.next_free, 16) != NULL);
	last->nb_alloc++;
	show_alloc_mem();
	printf("2 allocs should be displayed\n");
	printf("\n>>>>>>>>> END TEST show_alloc_mem_empty_bin_in_the_middle <<<<<<<<<\n");

}