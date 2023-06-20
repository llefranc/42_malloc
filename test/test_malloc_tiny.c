/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_malloc_tiny.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 15:22:22 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/20 17:28:32 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/bins.h"
#include "../src/allocator.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

const size_t tiny_min_nb = 1;
const size_t tiny_max_nb = TINY_MAX_ALLOC_SIZE;
const size_t tiny_bin_size = TINY_MMAP_SIZE % 4096 ?
	TINY_MMAP_SIZE + (4096 - TINY_MMAP_SIZE % 4096) : TINY_MMAP_SIZE;

void test_malloc_tiny_one_alloc_min_size(void)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_tiny_one_alloc_min_size <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(tiny_min_nb)) != NULL);
	memset(ptr, letter++, chk_size_16align(tiny_min_nb));
	show_alloc_mem_ex(bins.tiny, 4096);
	print_free_chunks(&bins.tiny->first_free);
	clear_bins();
	printf("\n>>>>>>>>> END TEST malloc_tiny_one_alloc_min_size <<<<<<<<<\n");
}

void test_malloc_tiny_one_alloc_max_size(void)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_tiny_one_alloc_max_size <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(tiny_max_nb)) != NULL);
	memset(ptr, letter++, chk_size_16align(tiny_max_nb));
	show_alloc_mem_ex(bins.tiny, 4096);
	print_free_chunks(&bins.tiny->first_free);
	clear_bins();
	printf("\n>>>>>>>>> END TEST malloc_tiny_one_alloc_max_size <<<<<<<<<\n");
}

void test_malloc_tiny_fill_one_mmap(void)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_tiny_fill_one_mmap <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	for (size_t i = 0; i < tiny_bin_size / tiny_max_nb + 5; ++i) {
		assert((ptr = ft_malloc(tiny_max_nb)) != NULL);
		memset(ptr, letter++, chk_size_16align(tiny_max_nb));
	}
	show_alloc_mem_ex(bins.tiny->next, 320);
	printf("first mmap:\n");
	print_free_chunks(&bins.tiny->first_free);
	printf("second mmap:\n");
	print_free_chunks(&bins.tiny->next->first_free);
	show_alloc_mem();
	clear_bins();
	printf("\n>>>>>>>>> END TEST malloc_tiny_fill_one_mmap <<<<<<<<<\n");
}

void test_malloc_tiny_all_values_from_tiny_range(void)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_tiny_all_values_from_tiny_range <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	int i = 1;

	for (size_t size = tiny_min_nb; size < tiny_max_nb; ++size) {
		printf("alloc nb %d: allocating size %zu\n", i++, size);
		assert((ptr = ft_malloc(size)) != NULL);
		memset(ptr, letter++, chk_size_16align(size));
	}
	printf("all alloc success\n");
	show_alloc_mem_ex(bins.tiny, tiny_bin_size);
	show_alloc_mem();
	clear_bins();
	printf("\n>>>>>>>>> END TEST malloc_tiny_all_values_from_tiny_range <<<<<<<<<\n");
}