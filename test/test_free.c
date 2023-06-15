/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 18:45:50 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/15 15:43:15 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/bins.h"
#include "../src/allocator.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_free_one_tiny(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_one_tiny <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	assert((ptr = ft_malloc(1)) != NULL);
	printf("allocating one tiny chunk\n");
	memset(ptr, letter++, chk_size_16align(1));
	lmmap_print_all(bins.tiny);
	show_alloc_mem();
	print_mem(bins.tiny, 128);

	printf("\nfreeing one tiny chunk\n");
	ft_free(ptr);
	assert(lmmap_get_size(bins.tiny) == 1);
	assert(bins.tiny->first_free.next_free->next_free == NULL);
	printf("Elem successfully free, only one big free chunk in tiny bin\n");
	show_alloc_mem();
	print_mem(bins.tiny, 128);
	printf("\n>>>>>>>>> END TEST free_one_tiny <<<<<<<<<\n");
}

void test_free_all_tiny_range(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_all_tiny_range <<<<<<<<<\n");
	static char letter = 'A';
	char *arr[TINY_MAX_ALLOC_SIZE];
	size_t size = 1;
	int i = 0;

	while (size <= TINY_MAX_ALLOC_SIZE) {
		assert((arr[i] = ft_malloc(size)) != NULL);
		memset(arr[i++], letter++, chk_size_16align(size++));
	}
	i--;
	lmmap_print_all(bins.tiny);
	show_alloc_mem();

	printf("\nfreeing all tiny chunks\n");
	while (i >= 0)
		ft_free(arr[i--]);
	show_alloc_mem();
	struct mmaphdr *elem = bins.tiny;
	while (elem) {
		assert(elem->first_free.next_free->next_free == NULL);
		elem = elem->next;
	}
	print_mem(bins.tiny, 1024);
	printf("all allocated chunks successfully freed\n");
	printf("\n>>>>>>>>> END TEST free_all_tiny_range <<<<<<<<<\n");
}

void test_free_one_small(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_one_small <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	assert((ptr = ft_malloc(TINY_MAX_ALLOC_SIZE + 1)) != NULL);
	printf("allocating one tiny chunk\n");
	memset(ptr, letter++, chk_size_16align(TINY_MAX_ALLOC_SIZE + 1));
	lmmap_print_all(bins.small);
	show_alloc_mem();
	print_mem(bins.small, 128);

	printf("\nfreeing one tiny chunk\n");
	ft_free(ptr);
	assert(lmmap_get_size(bins.small) == 1);
	assert(bins.small->first_free.next_free->next_free == NULL);
	printf("Elem successfully free, only one big free chunk in tiny bin\n");
	show_alloc_mem();
	print_mem(bins.small, 128);
	printf("\n>>>>>>>>> END TEST free_one_small <<<<<<<<<\n");
}

void test_free_all_small_range(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_all_small_range <<<<<<<<<\n");
	static char letter = 'A';
	char *arr[SMALL_MAX_ALLOC_SIZE - TINY_MAX_ALLOC_SIZE];
	size_t size = TINY_MAX_ALLOC_SIZE + 1;
	int i = 0;

	while (size <= SMALL_MAX_ALLOC_SIZE) {
		assert((arr[i] = ft_malloc(size)) != NULL);
		memset(arr[i++], letter++, chk_size_16align(size++));
	}
	i--;
	lmmap_print_all(bins.small);
	show_alloc_mem();

	printf("\nfreeing all tiny chunks\n");
	while (i >= 0)
		ft_free(arr[i--]);
	show_alloc_mem();
	struct mmaphdr *elem = bins.small;
	while (elem) {
		assert(elem->first_free.next_free->next_free == NULL);
		elem = elem->next;
	}
	print_mem(bins.small, 1024);
	printf("all allocated chunks successfully freed\n");
	printf("\n>>>>>>>>> END TEST free_all_small_range <<<<<<<<<\n");
}

void test_free_one_large(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_one_large <<<<<<<<<\n");
	size_t large_size = SMALL_MAX_ALLOC_SIZE + 1;

	static char letter = 'A';
	char *ptr;
	printf("allocating a large malloc. Size=%zu\n", large_size);
	assert((ptr = ft_malloc(large_size)) != NULL);
	memset(ptr, letter++, chk_size_16align(large_size));
	lmmap_print_all(bins.large);
	show_alloc_mem();
	print_mem(bins.large, 128);
	printf("freeing the large chunk\n");
	ft_free(ptr);
	assert(bins.large == NULL);
	printf("Elem successfully free and bins.large correctly set to NULL\n");
	printf("\n>>>>>>>>> END TEST free_one_large <<<<<<<<<\n");
}

void test_free_several_large(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_several_large <<<<<<<<<\n");
	size_t large_size = SMALL_MAX_ALLOC_SIZE + 1;

	static char letter = 'A';
	char *ptr1;
	char *ptr2;
	char *ptr3;
	printf("allocating 3 large malloc\n");
	assert((ptr1 = ft_malloc(large_size)) != NULL);
	memset(ptr1, letter++, chk_size_16align(large_size));
	large_size *= 10;
	assert((ptr2 = ft_malloc(large_size)) != NULL);
	memset(ptr2, letter++, chk_size_16align(large_size));
	large_size *= 10;
	assert((ptr3 = ft_malloc(large_size)) != NULL);
	memset(ptr3, letter++, chk_size_16align(large_size));
	lmmap_print_all(bins.large);
	assert(lmmap_get_size(bins.large) == 3);
	show_alloc_mem();

	printf("\nfreeing 1 large malloc\n");
	ft_free(ptr2);
	assert(lmmap_get_size(bins.large) == 2);
	show_alloc_mem();

	printf("\nfreeing 1 large mallocs\n");
	ft_free(ptr1);
	assert(lmmap_get_size(bins.large) == 1);
	show_alloc_mem();

	printf("\nfreeing last large malloc\n");
	ft_free(ptr3);
	assert(bins.large == NULL);
	show_alloc_mem();

	printf("\nElems successfully freed and bins.large correctly set to NULL\n");
	printf("\n>>>>>>>>> END TEST free_several_large <<<<<<<<<\n");
}

void test_free_empty_bins(void)
{
	printf("\n>>>>>>>>> BEGIN TEST free_empty_all <<<<<<<<<\n");
	void *ptr[500] = {};
	int nb_allocs = 500;

	for (int i = 0; i  < nb_allocs; ++i) {
		assert((ptr[i] = ft_malloc(TINY_MAX_ALLOC_SIZE)) != NULL);
	}
	show_alloc_mem();
	printf("Number of tiny bins %d\n", lmmap_get_size(bins.tiny));
	for (int i = 0; i < nb_allocs; ++i)
		ft_free(ptr[i]);
	printf("All malloc succesfully freed\n");
	printf("Number of tiny bins %d\n", lmmap_get_size(bins.tiny));

	assert(lmmap_get_size(bins.tiny) == 1);
	clear_bins();

	printf("\n>>>>>>>>> END TEST free_empty_all <<<<<<<<<\n");	
}
