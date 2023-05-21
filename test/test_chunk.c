/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 10:34:48 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/21 20:34:00 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/chunk.h"
#include "../src/lmmap.h"

#include <stdio.h>
#include <assert.h>

void test_chk_get_alloc_size(void)
{
	printf("\n>>>>>>>>> BEGIN TEST chk_get_alloc_size <<<<<<<<<\n");
	for (int i = 0; i < 40; ++i) {
		printf("aligned size for %d = %zu\n", i, chk_get_alloc_size(i));
	}
	printf("\n>>>>>>>>> END TEST chk_get_alloc_size <<<<<<<<<\n");
}

void test_chk_is_alloc_ok(void)
{
	printf("\n>>>>>>>>> BEGIN TEST chk_is_alloc_ok <<<<<<<<<\n");
	struct chkhdr hdr;
	size_t size = 5;

	size = chk_get_alloc_size(size);

	hdr.size = 30;
	for (; hdr.size < 55; hdr.size++) {
		printf("free chunk size = %zu, trying to alloc %zu bytes, is_fitting: "
		       "%d\n", (size_t)hdr.size, size, chk_is_alloc_ok(&hdr, size));
	}

	printf("\n>>>>>>>>> END TEST chk_is_alloc_ok <<<<<<<<<\n");
}

void test_chk_alloc(void)
{
	printf("\n>>>>>>>>> BEGIN TEST chk_alloc <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	struct chkhdr *tmp;
	lmmap_print_all(m);

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	/* Several alloc that fills */
	assert((tmp = chk_alloc(m->first_free.next_free, 16)) != NULL);
	chk_print(tmp);
	assert((tmp = chk_alloc(m->first_free.next_free, 48)) != NULL);
	chk_print(tmp);
	assert((tmp = chk_alloc(m->first_free.next_free, 32)) != NULL);
	chk_print(tmp);
	assert((tmp = chk_alloc(m->first_free.next_free, 3808)) != NULL);

	// Alloc should failed because too large
	assert((tmp = chk_alloc(m->first_free.next_free, 128)) == NULL);
	assert((tmp = chk_alloc(m->first_free.next_free, 40)) != NULL);
	chk_print(tmp);

	printf("\n>>>>>>>>> END TEST chk_alloc <<<<<<<<<\n");
	print_mem(m, 4096);
}

void test_chk_moves(void)
{
	printf("\n>>>>>>>>> BEGIN TEST chk_moves <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	/* Several alloc that fills */
	assert(chk_alloc(m->first_free.next_free, 16) != NULL);
	assert(chk_alloc(m->first_free.next_free, 48) != NULL);
	assert(chk_alloc(m->first_free.next_free, 32) != NULL);

	struct chkhdr *hdr = m->first_chk;
	struct chkftr *ftr;
	while (hdr != m->first_free.next_free) {
		chk_print(hdr);
		ftr = chk_htof(hdr);
		hdr = chk_ftoh(ftr);
		hdr = chk_next_hdr(hdr);
	}
	ftr = chk_prev_ftr(m->last_chk);
	hdr = chk_ftoh(ftr);
	printf("changing sides\n");
	while (hdr != m->first_chk) {
		hdr = chk_ftoh(ftr);
		chk_print(hdr);
		ftr = chk_htof(hdr);
		ftr = chk_prev_ftr(ftr);
	}
	print_mem(m, 4096);
	printf("\n>>>>>>>>> END TEST chk_moves <<<<<<<<<\n");
}

static void print_free_chunks(struct chkhdr *first_free)
{
	struct chkhdr *elem = first_free;
	int i = 0;
	printf("printing linked list of free blocks:\n");
	while (elem != NULL) {
		printf("-------------\n");
		printf("free chunk nb %d\n", i++);
		printf("address of free chunk: %p\n", elem);
		printf("size of free chunk: %zu\n", (size_t)elem->size);
		printf("address of prev free chunk: %p\n", elem->prev_free);
		printf("address of next free chunk: %p\n", elem->next_free);
		printf("-------------\n");
		elem = elem->next_free;
	}
}

void test_chk_free_basics(void)
{
	printf("\n>>>>>>>>> BEGIN TEST chk_free_basics <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	struct chkhdr *hdr;
	assert((hdr = chk_alloc(m->first_free.next_free, 16)) != NULL);

	printf("-------- mmap after one alloc of 16 bytes --------\n");
	print_mem(m, 128);
	printf("-------- mmap after one alloc of 16 bytes --------\n");
	print_free_chunks(&m->first_free);

	assert(chk_free(hdr, m->first_chk, m->last_chk, &m->first_free) != NULL);

	printf("-------- mmap after free --------\n");
	print_mem(m, 128);
	printf("-------- mmap after free --------\n");
	print_free_chunks(&m->first_free);

	assert((hdr = chk_alloc(m->first_free.next_free, 32)) != NULL);

	printf("-------- mmap after one alloc of 24 bytes --------\n");
	print_mem(m, 4096);
	printf("-------- mmap after one alloc of 24 bytes --------\n");
	print_free_chunks(&m->first_free);

	printf("\n>>>>>>>>> END TEST chk_free_basics <<<<<<<<<\n");
}

/* To test the limits of the freed chunk linked list */
void test_chk_free_no_merge_one_big_chunk(void)
{
	printf("\n>>>>>>>>> BEGIN TEST test_chk_free_no_merge_one_big_chunk <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	struct chkhdr *hdr;
	assert((hdr = chk_alloc(m->first_free.next_free, 4008)) != NULL);
	printf("-------- mmap after one alloc of 4008 bytes --------\n");
	print_mem(m, 128);
	printf("-------- mmap after one alloc of 4008 bytes --------\n");


	assert((hdr = chk_free(hdr, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free\n", hdr);
	printf("-------- mmap after free --------\n");
	print_mem(m, 4096);
	printf("-------- mmap after free --------\n");
	print_free_chunks(&m->first_free);

	printf("\n>>>>>>>>> END TEST test_chk_free_no_merge_one_big_chunk <<<<<<<<<\n");
}

void test_chk_free_no_merge_several_small_chunks(void)
{
	printf("\n>>>>>>>>> BEGIN TEST test_chk_free_no_merge_several_small_chunks <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	struct chkhdr *hdr1;
	struct chkhdr *hdr2;
	struct chkhdr *hdr3;
	struct chkhdr *hdr4;
	struct chkhdr *hdr5;
	struct chkhdr *hdr6;

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	assert((hdr1 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr2 = chk_alloc(m->first_free.next_free, 32)) != NULL);
	assert((hdr3 = chk_alloc(m->first_free.next_free, 48)) != NULL);
	assert((hdr4 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr5 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr6 = chk_alloc(m->first_free.next_free, 32)) != NULL);

	printf("-------- mmap after 6 allocs --------\n");
	print_mem(m, 368);
	printf("-------- mmap after 6 allocs --------\n");

	assert((hdr2 = chk_free(hdr2, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr2, (size_t)hdr2->size);

	assert((hdr5 = chk_free(hdr5, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr5, (size_t)hdr5->size);

	printf("-------- mmap after free block 2 and 5 --------\n");
	print_mem(m, 368);
	printf("-------- mmap after free block 2 and 5 --------\n");
	print_free_chunks(&m->first_free);


	printf("\n>>>>>>>>> END TEST test_chk_free_no_merge_several_small_chunks <<<<<<<<<\n");
}

void test_chk_free_merge_prev(void)
{
	printf("\n>>>>>>>>> BEGIN TEST test_chk_free_merge_prev <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	struct chkhdr *hdr1;
	struct chkhdr *hdr2;
	struct chkhdr *hdr3;
	struct chkhdr *hdr4;
	struct chkhdr *hdr5;
	struct chkhdr *hdr6;

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	assert((hdr1 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr2 = chk_alloc(m->first_free.next_free, 32)) != NULL);
	assert((hdr3 = chk_alloc(m->first_free.next_free, 48)) != NULL);
	assert((hdr4 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr5 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr6 = chk_alloc(m->first_free.next_free, 32)) != NULL);

	printf("-------- mmap after 6 allocs --------\n");
	print_mem(m, 368);
	printf("-------- mmap after 6 allocs --------\n");

	printf("freeing first chunk\n");
	assert((hdr3 = chk_free(hdr3, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr3, (size_t)hdr3->size);

	printf("freeing another chunk after the first one freed, will be"
	       " merge with previous\n");
	assert((hdr4 = chk_free(hdr4, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr4, (size_t)hdr4->size);

	printf("freeing another chunk after the first two freed, will be"
	       " merge with previous chunk (resulting from previous merge)\n");
	assert((hdr5 = chk_free(hdr5, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr5, (size_t)hdr5->size);

	printf("-------- mmap after free block 3, 4 and 5 --------\n");
	print_mem(m, 368);
	printf("-------- mmap after free block 3, 4 and 5 --------\n");
	print_free_chunks(&m->first_free);

	printf("\n>>>>>>>>> END TEST test_chk_free_merge_prev <<<<<<<<<\n");
}

void test_chk_free_merge_next(void)
{
	printf("\n>>>>>>>>> BEGIN TEST test_chk_free_merge_next <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	struct chkhdr *hdr1;
	struct chkhdr *hdr2;
	struct chkhdr *hdr3;
	struct chkhdr *hdr4;
	struct chkhdr *hdr5;
	struct chkhdr *hdr6;

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	assert((hdr1 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr2 = chk_alloc(m->first_free.next_free, 32)) != NULL);
	assert((hdr3 = chk_alloc(m->first_free.next_free, 48)) != NULL);
	assert((hdr4 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr5 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr6 = chk_alloc(m->first_free.next_free, 32)) != NULL);

	printf("-------- mmap after 6 allocs --------\n");
	print_mem(m, 368);
	printf("-------- mmap after 6 allocs --------\n");

	printf("freeing first chunk\n");
	assert((hdr4 = chk_free(hdr4, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr4, (size_t)hdr4->size);

	printf("freeing another chunk before the first one freed, will be"
	       " merge with next\n");
	assert((hdr3 = chk_free(hdr3, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr3, (size_t)hdr3->size);

	printf("freeing another chunk before the first two freed, will be"
	       " merge with next chunk (resulting from next merge)\n");
	assert((hdr2 = chk_free(hdr2, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr2, (size_t)hdr2->size);

	printf("-------- mmap after free block 2, 3 and 4 --------\n");
	print_mem(m, 368);
	printf("-------- mmap after free block 2, 3 and 4 --------\n");
	print_free_chunks(&m->first_free);

	printf("\n>>>>>>>>> END TEST test_chk_free_merge_next <<<<<<<<<\n");
}

void test_chk_free_merge_prev_and_next(void)
{
	printf("\n>>>>>>>>> BEGIN TEST test_chk_free_merge_prev_and_next <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	struct chkhdr *hdr1;
	struct chkhdr *hdr2;
	struct chkhdr *hdr3;
	struct chkhdr *hdr4;
	struct chkhdr *hdr5;
	struct chkhdr *hdr6;

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	assert((hdr1 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr2 = chk_alloc(m->first_free.next_free, 32)) != NULL);
	assert((hdr3 = chk_alloc(m->first_free.next_free, 48)) != NULL);
	assert((hdr4 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr5 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr6 = chk_alloc(m->first_free.next_free, 32)) != NULL);

	printf("-------- mmap after 6 allocs --------\n");
	print_mem(m, 368);
	printf("-------- mmap after 6 allocs --------\n");

	printf("freeing first chunk (chunk 2)\n");
	assert((hdr2 = chk_free(hdr2, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr2, (size_t)hdr2->size);

	printf("freeing another chunk (chunk 4, no merge)\n");
	assert((hdr4 = chk_free(hdr4, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr4, (size_t)hdr4->size);

	printf("freeing chunk 3, between the two previous freed chunks (will "
	       "be merge with prev and next)\n");
	assert((hdr3 = chk_free(hdr3, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free (size: %zu)\n", hdr3, (size_t)hdr3->size);

	printf("-------- mmap after free block 2, 3 and 4 --------\n");
	print_mem(m, 368);
	printf("-------- mmap after free block 2, 3 and 4 --------\n");
	print_free_chunks(&m->first_free);

	printf("\n>>>>>>>>> END TEST test_chk_free_merge_prev_and_next <<<<<<<<<\n");
}

void test_chk_free_all_with_all_cases(void)
{
	printf("\n>>>>>>>>> BEGIN TEST test_chk_free_all_with_all_cases <<<<<<<<<\n");
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	struct chkhdr *tmp;
	struct chkhdr *hdr1;
	struct chkhdr *hdr2;
	struct chkhdr *hdr3;
	struct chkhdr *hdr4;
	struct chkhdr *hdr5;
	struct chkhdr *hdr6;

	printf("-------- mmapheader before alloc --------\n");
	print_mem(m, 128);
	printf("-------- mmapheader before alloc --------\n");

	assert((hdr1 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr2 = chk_alloc(m->first_free.next_free, 32)) != NULL);
	assert((hdr3 = chk_alloc(m->first_free.next_free, 48)) != NULL);
	assert((hdr4 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr5 = chk_alloc(m->first_free.next_free, 16)) != NULL);
	assert((hdr6 = chk_alloc(m->first_free.next_free, 32)) != NULL);

	printf("-------- mmap after 6 allocs --------\n");
	print_mem(m, 368);
	printf("-------- mmap after 6 allocs --------\n");

	print_free_chunks(&m->first_free);


	printf("freeing chunk 2\n");
	assert((tmp = chk_free(hdr2, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free, size %zu\n", tmp, (size_t)tmp->size);
	printf("freeing chunk 3\n");
	assert((tmp = chk_free(hdr3, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free, size %zu\n", tmp, (size_t)tmp->size);
	printf("freeing chunk 1\n");
	assert((tmp = chk_free(hdr1, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free, size %zu\n", tmp, (size_t)tmp->size);
	printf("freeing chunk 6\n");
	assert((tmp = chk_free(hdr6, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free, size %zu\n", tmp, (size_t)tmp->size);
	printf("freeing chunk 5\n");
	assert((tmp = chk_free(hdr5, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free, size %zu\n", tmp, (size_t)tmp->size);
	printf("freeing chunk 4\n");
	assert((tmp = chk_free(hdr4, m->first_chk, m->last_chk, &m->first_free)) != NULL);
	printf("chunk at address %p is now free, size %zu\n", tmp, (size_t)tmp->size);

	printf("-------- mmap after freeing all blocks --------\n");
	print_mem(m, 4096);
	printf("-------- mmap after freeing all blocks --------\n");
	print_free_chunks(&m->first_free);

	printf("\n>>>>>>>>> END TEST test_chk_free_all_with_all_cases <<<<<<<<<\n");
}