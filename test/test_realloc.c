/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_realloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:19:49 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/22 18:02:19 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/allocator.h"
#include "../src/lmmap.h"
#include "../src/bins.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

void test_realloc_ptr_null(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_ptr_null <<<<<<<<<\n");
	assert(ft_realloc(NULL, 0) != NULL);
	assert(ft_realloc(NULL, TINY_MAX_ALLOC_SIZE) != NULL);
	assert(ft_realloc(NULL, TINY_MAX_ALLOC_SIZE + 1) != NULL);
	assert(ft_realloc(NULL, SMALL_MAX_ALLOC_SIZE) != NULL);
	assert(ft_realloc(NULL, SMALL_MAX_ALLOC_SIZE + 1) != NULL);
	show_alloc_mem();
	assert(bins.tiny->nb_alloc == 2);
	assert(bins.small->nb_alloc == 2);
	assert(bins.large->nb_alloc == 1);
	printf("\n>>>>>>>>> END TEST realloc_ptr_null <<<<<<<<<\n");
}

void test_realloc_size_0(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_size_0 <<<<<<<<<\n");
	void *ptr_tiny;
	void *ptr_small;
	void *ptr_large;

	assert((ptr_tiny = ft_malloc(0)) != NULL);
	assert((ptr_small = ft_malloc(TINY_MAX_ALLOC_SIZE + 1)) != NULL);
	assert((ptr_large = ft_malloc(SMALL_MAX_ALLOC_SIZE + 1)) != NULL);
	assert(bins.tiny->nb_alloc == 1);
	assert(bins.small->nb_alloc == 1);
	assert(bins.large->nb_alloc == 1);
	printf("Before using realloc for freeing:\n");
	show_alloc_mem();

	printf("\nAfter using realloc for freeing all allocated areas:\n");
	assert(ft_realloc(ptr_tiny, 0) == NULL);
	assert(ft_realloc(ptr_small, 0) == NULL);
	assert(ft_realloc(ptr_large, 0) == NULL);
	assert(bins.tiny->nb_alloc == 0);
	assert(bins.small->nb_alloc == 0);
	assert(bins.large == NULL);
	show_alloc_mem();

	printf("\n>>>>>>>>> END TEST realloc_size_0 <<<<<<<<<\n");
}

void test_realloc_inc_not_in_same_bin(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_inc_not_in_same_bin <<<<<<<<<\n");
	void *ptr;

	assert((ptr = ft_malloc(10)) != NULL);
	memset(ptr, 'A', 10);
	printf("\nallocating first in tiny:");
	show_alloc_mem_ex(bins.tiny, 128);
	show_alloc_mem();

	printf("\nrealloc tiny to small:");
	assert((ptr = ft_realloc(ptr, TINY_MAX_ALLOC_SIZE + 1)) != NULL);
	assert(bins.tiny->nb_alloc == 0);
	assert(bins.small->nb_alloc == 1);
	show_alloc_mem_ex(bins.small, 128);
	show_alloc_mem();

	printf("\nrealloc small to large:");
	assert((ptr = ft_realloc(ptr, SMALL_MAX_ALLOC_SIZE + 1)) != NULL);
	assert(bins.tiny->nb_alloc == 0);
	assert(bins.small->nb_alloc == 0);
	assert(lmmap_get_size(bins.large) == 1);
	show_alloc_mem_ex(bins.large, 128);
	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST realloc_inc_not_in_same_bin <<<<<<<<<\n");
}

void test_realloc_dec_not_in_same_bin(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_not_in_same_bin <<<<<<<<<\n");
	void *ptr;

	assert((ptr = ft_malloc(SMALL_MAX_ALLOC_SIZE + 1)) != NULL);
	memset(ptr, 'A', SMALL_MAX_ALLOC_SIZE + 1);
	printf("\nallocating first in large:");
	show_alloc_mem_ex(bins.large, 256);
	show_alloc_mem();

	printf("\nrealloc large to small:");
	assert((ptr = ft_realloc(ptr, TINY_MAX_ALLOC_SIZE + 1)) != NULL);
	assert(bins.small->nb_alloc == 1);
	assert(bins.large == NULL);
	show_alloc_mem_ex(bins.small, 256);
	show_alloc_mem();

	printf("\nrealloc small to tiny:");
	assert((ptr = ft_realloc(ptr, 10)) != NULL);
	assert(bins.tiny->nb_alloc == 1);
	assert(bins.small->nb_alloc == 0);
	assert(bins.large == NULL);
	show_alloc_mem_ex(bins.tiny, 256);
	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST realloc_dec_not_in_same_bin <<<<<<<<<\n");
}

void test_realloc_same_size(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_same_size <<<<<<<<<\n");
	void *ptr_tiny;
	void *ptr_small;
	void *ptr_large;
	size_t size_tiny = 1;
	size_t size_small = TINY_MAX_ALLOC_SIZE + 1;
	size_t size_large = SMALL_MAX_ALLOC_SIZE + 1;

	assert((ptr_tiny = ft_malloc(size_tiny)) != NULL);
	assert((ptr_small = ft_malloc(size_small)) != NULL);
	assert((ptr_large = ft_malloc(size_large)) != NULL);
	assert(bins.tiny->nb_alloc == 1);
	assert(bins.small->nb_alloc == 1);
	assert(bins.large->nb_alloc == 1);

	assert(ft_realloc(ptr_tiny, size_tiny) == ptr_tiny);
	assert(ft_realloc(ptr_small, size_small) == ptr_small);
	assert(ft_realloc(ptr_large, size_large) == ptr_large);

	printf("Succesfully reallocated on same pointers with same size\n");
	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST realloc_same_size <<<<<<<<<\n");
}

void test_realloc_dec_but_not_enough_space_to_split(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_but_not_enough_space_to_split <<<<<<<<<\n");
	void *ptr_tiny;
	void *ptr_small;
	size_t size_tiny = 64;
	size_t size_small = TINY_MAX_ALLOC_SIZE + 64;

	printf("allocating a tiny chunk and a small chunk\n");
	assert((ptr_tiny = ft_malloc(size_tiny)) != NULL);
	assert((ptr_small = ft_malloc(size_small)) != NULL);
	assert(bins.tiny->nb_alloc == 1);
	assert(bins.small->nb_alloc == 1);
	show_alloc_mem();

	printf("Allocating size - 16: (should not change)\n");
	assert(ft_realloc(ptr_tiny, size_tiny - 16) == ptr_tiny);
	assert(ft_realloc(ptr_small, size_small - 16) == ptr_small);
	show_alloc_mem();

	printf("Succesfully reallocated on same pointers with small dec\n");
	printf("\n>>>>>>>>> END TEST realloc_dec_but_not_enough_space_to_split <<<<<<<<<\n");
}

void test_realloc_dec_split_chunk(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_split_chunk <<<<<<<<<\n");
	void *ptr_tiny;
	void *ptr_small;
	size_t size_tiny = 64;
	size_t size_small = TINY_MAX_ALLOC_SIZE + 64;

	printf("allocating a tiny chunk and a small chunk\n");
	assert((ptr_tiny = ft_malloc(size_tiny)) != NULL);
	memset(ptr_tiny, 'A', size_tiny);
	assert((ptr_small = ft_malloc(size_small)) != NULL);
	memset(ptr_small, 'B', size_small);
	assert(bins.tiny->nb_alloc == 1);
	assert(bins.small->nb_alloc == 1);
	show_alloc_mem();

	printf("Allocating size - 32: (should change)\n");
	assert(ft_realloc(ptr_tiny, size_tiny - 32) == ptr_tiny);
	assert(ft_realloc(ptr_small, size_small - 32) == ptr_small);

	printf("New tiny bin after realloc dec:\n");
	show_alloc_mem_ex(bins.tiny, 256);
	print_free_chunks(&bins.tiny->first_free);

	printf("New small bin after realloc dec:\n");
	show_alloc_mem_ex(bins.small, 1024);
	print_free_chunks(&bins.small->first_free);
	show_alloc_mem();

	printf("\n>>>>>>>>> END TEST realloc_dec_split_chunk <<<<<<<<<\n");
}

void test_realloc_dec_split_chunk_second_bin(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_split_chunk_second_bin <<<<<<<<<\n");
	void *ptr_tiny;
	struct mmaphdr *secbin;

	printf("Filling the first tiny bin with mallocs:\n");
	while (lmmap_get_size(bins.tiny) != 2)
		assert((ptr_tiny = ft_malloc(TINY_MAX_ALLOC_SIZE)) != NULL);
	memset(ptr_tiny, 'A', TINY_MAX_ALLOC_SIZE);
	show_alloc_mem();
	secbin = bins.tiny->next;

	printf("Printing info of second mmap:\n");
	assert(secbin->nb_alloc == 1);
	show_alloc_mem_ex(secbin, 256);
	print_free_chunks(&secbin->first_free);

	assert(ft_realloc(ptr_tiny, 1) == ptr_tiny);
	printf("Printing info of second mmap after realloc to 1 byte:\n");
	assert(secbin->nb_alloc == 1);
	show_alloc_mem_ex(secbin, 256);
	print_free_chunks(&secbin->first_free);
	show_alloc_mem();

	printf("\n>>>>>>>>> END TEST realloc_dec_split_chunk_second_bin <<<<<<<<<\n");
}

void test_realloc_dec_split_chunk_merge(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_split_chunk_merge <<<<<<<<<\n");
	void *ptr_tiny;
	void *ptr_free;
	void *ptr_last;
	size_t size_tiny = 64;

	printf("allocating 3 tiny chunks:\n");
	assert((ptr_tiny = ft_malloc(size_tiny)) != NULL);
	memset(ptr_tiny, 'A', size_tiny);

	assert((ptr_free = ft_malloc(size_tiny)) != NULL);
	memset(ptr_free, 'B', size_tiny);
	assert((ptr_last = ft_malloc(size_tiny)) != NULL);
	memset(ptr_last, 'C', size_tiny);
	assert(bins.tiny->nb_alloc == 3);
	show_alloc_mem();

	printf("Freeing one chunk just after realloc:\n");
	ft_free(ptr_free);
	assert(bins.tiny->nb_alloc == 2);
	show_alloc_mem_ex(bins.tiny, 512);
	print_free_chunks(&bins.tiny->first_free);
	show_alloc_mem();

	printf("Doing a realloc (size - 32), split chunk should be merge with next free chunk:\n");
	assert(ft_realloc(ptr_tiny, size_tiny - 32) == ptr_tiny);
	assert(chk_size(bins.tiny->first_chk->info) == size_tiny - 32);
	assert(bins.tiny->nb_alloc == 2);
	show_alloc_mem_ex(bins.tiny, 512);
	print_free_chunks(&bins.tiny->first_free);
	show_alloc_mem();

	printf("\n>>>>>>>>> END TEST realloc_dec_split_chunk_merge <<<<<<<<<\n");
}

void test_realloc_dec_then_inc_same_bin(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_then_inc_same_bin <<<<<<<<<\n");
	void *ptr_tiny;
	size_t size_tiny = 64;

	printf("Allocating one tiny chunl:\n");
	assert((ptr_tiny = ft_malloc(size_tiny)) != NULL);
	assert(bins.tiny->nb_alloc == 1);
	memset(ptr_tiny, 'A', size_tiny);
	show_alloc_mem_ex(bins.tiny, 256);
	print_free_chunks(&bins.tiny->first_free);
	show_alloc_mem();

	printf("Doing a realloc (size - 32), split chunk should be merge with next big free chunk:\n");
	assert(ft_realloc(ptr_tiny, size_tiny - 32) == ptr_tiny);
	assert(chk_size(bins.tiny->first_chk->info) == size_tiny - 32);
	assert(bins.tiny->nb_alloc == 1);
	show_alloc_mem_ex(bins.tiny, 256);
	print_free_chunks(&bins.tiny->first_free);
	show_alloc_mem();

	printf("Incrementing the size to its original size, should free and alloc new chunk:\n");
	void *res;
	assert((res = ft_realloc(ptr_tiny, size_tiny)) != ptr_tiny);
	memset(res, 'B', size_tiny);
	assert(bins.tiny->nb_alloc == 1);
	show_alloc_mem_ex(bins.tiny, 256);
	print_free_chunks(&bins.tiny->first_free);
	show_alloc_mem();

	printf("\n>>>>>>>>> END TEST realloc_dec_then_inc_same_bin <<<<<<<<<\n");
}

void test_realloc_dec_in_same_page_does_nothing(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_in_same_page_does_nothing <<<<<<<<<\n");
	void *ptr_large;
	size_t large_size = SMALL_MAX_ALLOC_SIZE + 2000;

	printf("Allocating one large of %zu bytes:\n", large_size);
	assert((ptr_large = ft_malloc(large_size)) != NULL);

	assert(lmmap_get_size(bins.large) == 1);
	assert(bins.large->nb_alloc == 1);
	memset(ptr_large, 'A', large_size);
	show_alloc_mem();

	large_size -= 1000;
	printf("Doing a realloc of %zu bytes:\n", large_size);
	assert(ft_realloc(ptr_large, large_size) == ptr_large);
	assert(bins.large->nb_alloc == 1);
	memset(ptr_large, 'B', large_size);
	show_alloc_mem_ex(bins.large, 4096);
	show_alloc_mem();

	printf("\n>>>>>>>>> END TEST realloc_dec_in_same_page_does_nothing <<<<<<<<<\n");
}

void test_realloc_dec_trim_pages(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_trim_pages <<<<<<<<<\n");
	void *ptr_large;
	size_t large_size = SMALL_MAX_ALLOC_SIZE + 10000;

	printf("Allocating one large of %zu bytes:\n", large_size);
	assert((ptr_large = ft_malloc(large_size)) != NULL);

	assert(lmmap_get_size(bins.large) == 1);
	assert(bins.large->nb_alloc == 1);
	memset(ptr_large, 'A', large_size);
	show_alloc_mem();

	printf("Doing a realloc of %u bytes:\n", SMALL_MAX_ALLOC_SIZE + 2000);
	assert(ft_realloc(ptr_large, SMALL_MAX_ALLOC_SIZE + 2000) == ptr_large);
	assert(bins.large->nb_alloc == 1);
	memset(ptr_large, 'B', 2000);
	show_alloc_mem_ex(bins.large, 4096);

	show_alloc_mem();
	printf("\n>>>>>>>>> END TEST realloc_dec_trim_pages <<<<<<<<<\n");
}

static void assert_right_bin(void *ptr, size_t size)
{
	struct chkhdr *hdr = (struct chkhdr *)((uint8_t *)ptr - BNDARY_TAG_SIZE);
	struct chkftr *ftr = chk_htof(hdr);

	if (size <= TINY_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 1);
		assert(lmmap_get_size(bins.small) == 0);
		assert(lmmap_get_size(bins.large) == 0);
		assert(bins.tiny->nb_alloc == 1);
		assert(chk_size(hdr->info) == chk_size_16align(size));
		assert(chk_size(ftr->info) == chk_size_16align(size));
	} else if (size <= SMALL_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 1);
		assert(lmmap_get_size(bins.small) == 1);
		assert(lmmap_get_size(bins.large) == 0);
		assert(bins.tiny->nb_alloc == 0);
		assert(bins.small->nb_alloc == 1);
		assert(chk_size(hdr->info) == chk_size_16align(size));
		assert(chk_size(ftr->info) == chk_size_16align(size));
	} else {
		assert(lmmap_get_size(bins.tiny) == 1);
		assert(lmmap_get_size(bins.small) == 1);
		assert(lmmap_get_size(bins.large) == 1);
		assert(bins.tiny->nb_alloc == 0);
		assert(bins.small->nb_alloc == 0);
	}
}

void test_realloc_inc_first_bin_range_all_values(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_inc_first_bin_range_all_values <<<<<<<<<\n");
	void *ptr;
	size_t size = 1;
	char letter = 'A';
	int nb_pages = 0;

	assert((ptr = ft_malloc(size)) != NULL);
	memset(ptr, 'A', size);
	++size;
	while (size <= SMALL_MAX_ALLOC_SIZE + 6000) {
		void *old_ptr = ptr;
		printf("allocating %zu bytes\n", size);
		assert((ptr = ft_realloc(ptr, size)) != NULL);
		memset(ptr, letter, size);

		/* checking if realloc move tiny/small data when it can't fit anymore */
		if (chk_size_16align(size - 1) != chk_size_16align(size) &&
		    size <= SMALL_MAX_ALLOC_SIZE) {
			assert(old_ptr != ptr);

		/* checking if realloc move large data  when it can't fit anymore */
		} else if (size > SMALL_MAX_ALLOC_SIZE) {
			int new_nb_pages = bins.large->size / 4096;
			if (new_nb_pages != nb_pages)
				assert(old_ptr != ptr);
			else
				assert(old_ptr == ptr);
			nb_pages = new_nb_pages;
		}
		// show_alloc_mem();
		assert_right_bin(ptr, size);
		++letter;
		++size;
	}
	show_alloc_mem_ex(bins.large, 2000);

	printf("\n>>>>>>>>> END TEST realloc_inc_first_bin_range_all_values <<<<<<<<<\n");
}

static void assert_right_second_bin(void *ptr, size_t size)
{
	struct chkhdr *hdr = (struct chkhdr *)((uint8_t *)ptr - BNDARY_TAG_SIZE);
	struct chkftr *ftr = chk_htof(hdr);

	if (size <= TINY_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 2);
		assert(lmmap_get_size(bins.small) == 2);
		assert(lmmap_get_size(bins.large) == 1);
		assert(bins.tiny->next->nb_alloc == 1);
		assert(bins.small->next->nb_alloc == 0);
		assert(chk_size(hdr->info) == chk_size_16align(size));
		assert(chk_size(ftr->info) == chk_size_16align(size));
	} else if (size <= SMALL_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 2);
		assert(lmmap_get_size(bins.small) == 2);
		assert(lmmap_get_size(bins.large) == 1);
		assert(bins.tiny->next->nb_alloc == 0);
		assert(bins.small->next->nb_alloc == 1);
		assert(chk_size(hdr->info) == chk_size_16align(size));
		assert(chk_size(ftr->info) == chk_size_16align(size));
	} else {
		assert(lmmap_get_size(bins.tiny) == 2);
		assert(lmmap_get_size(bins.small) == 2);
		assert(lmmap_get_size(bins.large) == 2);
		assert(bins.tiny->next->nb_alloc == 0);
		assert(bins.small->next->nb_alloc == 0);
	}
}

void test_realloc_inc_second_bin_range_all_values(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_second_bin_range_all_values <<<<<<<<<\n");
	void *ptr;

	// Filling first bin of tiny
	while (lmmap_get_size(bins.tiny) != 2)
		assert((ptr = ft_malloc(TINY_MAX_ALLOC_SIZE)) != NULL);
	ft_free(ptr);
	assert(ft_malloc(chk_size(bins.tiny->first_free.next_free->info)));
	assert(lmmap_get_size(bins.tiny) == 2);
	assert(bins.tiny->next->nb_alloc == 0);

	// Filling first bin of small
	while (lmmap_get_size(bins.small) != 2)
		assert((ptr = ft_malloc(SMALL_MAX_ALLOC_SIZE)) != NULL);
	ft_free(ptr);
	assert(ft_malloc(chk_size(bins.small->first_free.next_free->info)));
	assert(lmmap_get_size(bins.small) == 2);
	assert(bins.small->next->nb_alloc == 0);

	// Filling first bin of large
	assert(ft_malloc(SMALL_MAX_ALLOC_SIZE + 1) != NULL);
	assert(lmmap_get_size(bins.large) == 1);

	show_alloc_mem();

	size_t size = 1;
	char letter = 'A';
	int nb_pages = 0;

	assert((ptr = ft_malloc(size)) != NULL);
	memset(ptr, 'A', size);
	++size;
	while (size <= SMALL_MAX_ALLOC_SIZE + 6000) {
		void *old_ptr = ptr;
		printf("allocating %zu bytes\n", size);
		assert((ptr = ft_realloc(ptr, size)) != NULL);
		memset(ptr, letter, size);

		// checking if realloc move tiny/small data when it can't fit anymore
		if (chk_size_16align(size - 1) != chk_size_16align(size) &&
		    size <= SMALL_MAX_ALLOC_SIZE) {
			assert(old_ptr != ptr);

		// checking if realloc move large data  when it can't fit anymore
		} else if (size > SMALL_MAX_ALLOC_SIZE) {
			int new_nb_pages = bins.large->next->size / 4096;
			if (new_nb_pages != nb_pages)
				assert(old_ptr != ptr);
			else
				assert(old_ptr == ptr);
			nb_pages = new_nb_pages;
		}
		// show_alloc_mem();
		assert_right_second_bin(ptr, size);
		++letter;
		++size;
	}
	show_alloc_mem_ex(bins.large->next, 2000);
	printf("\n>>>>>>>>> END TEST realloc_dec_second_bin_range_all_values <<<<<<<<<\n");
}

void assert_dec_bin(size_t size)
{
	if (size <= TINY_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 1);
		assert(lmmap_get_size(bins.small) == 1);
		assert(lmmap_get_size(bins.large) == 0);
		assert(bins.tiny->nb_alloc == 1);
		assert(bins.small->nb_alloc == 0);
	} else if (size <= SMALL_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 0);
		assert(lmmap_get_size(bins.small) == 1);
		assert(lmmap_get_size(bins.large) == 0);
		assert(bins.small->nb_alloc == 1);
	} else {
		assert(lmmap_get_size(bins.tiny) == 0);
		assert(lmmap_get_size(bins.small) == 0);
		assert(lmmap_get_size(bins.large) == 1);
	}
}

void test_realloc_dec_first_bin_range_all_values(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST realloc_dec_first_bin_range_all_values <<<<<<<<<\n");
	void *ptr;
	size_t size = SMALL_MAX_ALLOC_SIZE + 6000;
	char letter = 'A';

	assert((ptr = ft_malloc(size)) != NULL);
	memset(ptr, 'A', size);
	--size;
	while (size > 0) {
		void *old_ptr = ptr;
		printf("allocating %zu bytes\n", size);
		assert((ptr = ft_realloc(ptr, size)) != NULL);
		memset(ptr, letter, size);
		assert_dec_bin(size);
		if (size != TINY_MAX_ALLOC_SIZE && size != SMALL_MAX_ALLOC_SIZE)
			assert(old_ptr == ptr);

		// show_alloc_mem();
		++letter;
		--size;
	}
	show_alloc_mem_ex(bins.tiny, 2000);

	printf("\n>>>>>>>>> END TEST realloc_dec_first_bin_range_all_values <<<<<<<<<\n");
}

void assert_dec_second_bin(size_t size)
{
	if (size <= TINY_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 2);
		assert(lmmap_get_size(bins.small) == 2);
		assert(lmmap_get_size(bins.large) == 1);
		assert(bins.tiny->next->nb_alloc == 1);
		assert(bins.small->next->nb_alloc == 0);
	} else if (size <= SMALL_MAX_ALLOC_SIZE) {
		assert(lmmap_get_size(bins.tiny) == 2);
		assert(lmmap_get_size(bins.small) == 2);
		assert(lmmap_get_size(bins.large) == 1);
		assert(bins.small->next->nb_alloc == 1);
	} else {
		assert(lmmap_get_size(bins.tiny) == 2);
		assert(lmmap_get_size(bins.small) == 2);
		assert(lmmap_get_size(bins.large) == 2);
	}
}

void test_realloc_dec_second_bin_range_all_values(void)
{
	clear_bins();
	printf("\n>>>>>>>>> BEGIN TEST test_realloc_dec_second_bin_range_all_values <<<<<<<<<\n");
	void *ptr;

	// Filling first bin of tiny
	while (lmmap_get_size(bins.tiny) != 2)
		assert((ptr = ft_malloc(TINY_MAX_ALLOC_SIZE)) != NULL);
	ft_free(ptr);
	assert(ft_malloc(chk_size(bins.tiny->first_free.next_free->info)));
	assert(lmmap_get_size(bins.tiny) == 2);
	assert(bins.tiny->next->nb_alloc == 0);

	// Filling first bin of small
	while (lmmap_get_size(bins.small) != 2)
		assert((ptr = ft_malloc(SMALL_MAX_ALLOC_SIZE)) != NULL);
	ft_free(ptr);
	assert(ft_malloc(chk_size(bins.small->first_free.next_free->info)));
	assert(lmmap_get_size(bins.small) == 2);
	assert(bins.small->next->nb_alloc == 0);

	// Filling first bin of large
	assert(ft_malloc(SMALL_MAX_ALLOC_SIZE + 1) != NULL);
	assert(lmmap_get_size(bins.large) == 1);

	size_t size = SMALL_MAX_ALLOC_SIZE + 6000;
	char letter = 'A';

	assert((ptr = ft_malloc(size)) != NULL);
	memset(ptr, 'A', size);
	--size;
	while (size > 0) {
		void *old_ptr = ptr;
		printf("allocating %zu bytes\n", size);
		assert((ptr = ft_realloc(ptr, size)) != NULL);
		memset(ptr, letter, size);
		assert_dec_second_bin(size);
		if (size != TINY_MAX_ALLOC_SIZE && size != SMALL_MAX_ALLOC_SIZE)
			assert(old_ptr == ptr);

		// show_alloc_mem();
		++letter;
		--size;
	}
	show_alloc_mem_ex(bins.tiny->next, 2000);

	printf("\n>>>>>>>>> END TEST test_realloc_dec_second_bin_range_all_values <<<<<<<<<\n");
}
