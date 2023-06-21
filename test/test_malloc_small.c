/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_malloc_small.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 16:05:22 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 11:29:19 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/bins.h"
#include "../src/allocator.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

const size_t small_min_nb = TINY_MAX_ALLOC_SIZE + 1;
const size_t small_max_nb = SMALL_MAX_ALLOC_SIZE;
const size_t small_bin_size = SMALL_MMAP_SIZE % 4096 ?
	SMALL_MMAP_SIZE + (4096 - SMALL_MMAP_SIZE % 4096) : SMALL_MMAP_SIZE;

void test_malloc_small_one_alloc_min_size(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_small_one_alloc_min_size <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(small_min_nb)) != NULL);
	memset(ptr, letter++, chk_size_16align(small_min_nb));
	show_alloc_mem_ex(bins.small, 4096);
	print_free_chunks(&bins.small->first_free);
	printf("\n>>>>>>>>> END TEST malloc_small_one_alloc_min_size <<<<<<<<<\n");
}

void test_malloc_small_one_alloc_max_size(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_small_one_alloc_max_size <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(small_max_nb)) != NULL);
	memset(ptr, letter++, chk_size_16align(small_max_nb));
	show_alloc_mem_ex(bins.small, 4096);
	print_free_chunks(&bins.small->first_free);
	printf("\n>>>>>>>>> END TEST malloc_small_one_alloc_max_size <<<<<<<<<\n");
}

void test_malloc_small_fill_one_mmap(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_small_fill_one_mmap <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	for (size_t i = 0; i < small_bin_size / small_max_nb + 5; ++i) {
		assert((ptr = ft_malloc(small_max_nb)) != NULL);
		memset(ptr, letter++, chk_size_16align(small_max_nb));
	}
	show_alloc_mem_ex(bins.small->next, 4096);
	printf("first mmap:\n");
	print_free_chunks(&bins.small->first_free);
	printf("second mmap:\n");
	print_free_chunks(&bins.small->next->first_free);
	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST malloc_small_fill_one_mmap <<<<<<<<<\n");
}

void test_malloc_small_all_values_from_small_range(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_small_all_values_from_small_range <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	int i = 1;

	for (size_t size = small_min_nb; size < small_max_nb; ++size) {
		printf("alloc nb %d: allocating size %zu\n", i++, size);
		assert((ptr = ft_malloc(size)) != NULL);
		memset(ptr, letter++, chk_size_16align(size));
	}
	printf("all alloc success\n");
	show_alloc_mem_ex(bins.small, small_bin_size);
	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST malloc_small_all_values_from_small_range <<<<<<<<<\n");
}