/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_malloc_large.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:32:00 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/22 17:15:09 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/bins.h"
#include "../src/allocator.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/mman.h>

const size_t large_nb = SMALL_MAX_ALLOC_SIZE + 16;

void test_malloc_large_one_alloc_one_page(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_large_one_alloc_one_page <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(large_nb)) != NULL);
	memset(ptr, letter++, large_nb);
	show_alloc_mem_ex(bins.large, 4096);
	print_free_chunks(&bins.large->first_free);
	printf("\n>>>>>>>>> END TEST malloc_large_one_alloc_one_page <<<<<<<<<\n");
}

void test_malloc_large_one_alloc_multi_pages(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_large_one_alloc_multi_pages <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(1000000)) != NULL);
	memset(ptr, letter++, 1000000);
	show_alloc_mem_ex(bins.large, 128);
	print_free_chunks(&bins.large->first_free);
	printf("\n>>>>>>>>> END TEST malloc_large_one_alloc_multi_pages <<<<<<<<<\n");
}

void test_malloc_large_one_exact_page_size(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_large_one_exact_page_size <<<<<<<<<\n");
	size_t alloc_size = 4096 - sizeof(struct mmaphdr) - BNDARY_TAG_SIZE;
	static char letter = 'A';
	char *ptr;

	assert((ptr = ft_malloc(alloc_size)) != NULL);
	memset(ptr, letter++, alloc_size);
	show_alloc_mem_ex(bins.large, 4128);
	print_free_chunks(&bins.large->first_free);
	printf("\n>>>>>>>>> END TEST malloc_large_one_exact_page_size <<<<<<<<<\n");
}

void test_malloc_large_several_allocs(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_large_several_allocs <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	size_t alloc_size = large_nb;

	for (size_t i = 1; i < 15; ++i) {
		printf("doing allocation nb %zu of size %zu\n", i, alloc_size);
		// assert((ptr = malloc(alloc_size)) != NULL);
		assert((ptr = ft_malloc(alloc_size)) != NULL);
		memset(ptr, letter++, alloc_size);
		alloc_size = alloc_size * 2;
	}
	struct mmaphdr *head = bins.large;
	int nb_allocs = 1;
	while (head) {
		printf("\n------------\n");
		printf("\nlarge alloc nb %d\n", nb_allocs++);
		printf("allocated size = %zu\n", chk_size(head->first_chk->info));
		show_alloc_mem_ex(head, 128);
		print_free_chunks(&head->first_free);
		head = head->next;
	}

	printf("\n>>>>>>>>> END TEST malloc_large_several_allocs <<<<<<<<<\n");
}

void test_malloc_large_n_allocs_with_n_munmaps(size_t nb_mallocs)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST malloc_large_n_allocs_with_n_munmaps <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	size_t alloc_size = large_nb;

	printf("doing %zu large mallocs, with each one being immediately munmapped\n", nb_mallocs);
	for (size_t i = 1; i < nb_mallocs; ++i) {
		alloc_size = SMALL_MAX_ALLOC_SIZE + i;
		// printf("doing allocation nb %7zu of size %7zu\n", i, alloc_size);
		// assert((ptr = malloc(alloc_size)) != NULL);
		assert((ptr = ft_malloc(alloc_size)) != NULL);
		memset(ptr, letter++, alloc_size);
		clear_bins();
		// printf("\x1B[%zuA", strlen("doing allocation nb xxxxxxx of size xxxxxxx\n"));
		fflush(stdout);
	}
	printf("successfully did %zu large mallocs\n", nb_mallocs);
	printf("\n>>>>>>>>> END TEST malloc_large_n_allocs_with_n_munmaps <<<<<<<<<\n");
}