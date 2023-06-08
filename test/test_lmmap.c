/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lmmap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/08 17:04:50 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/lmmap.h"

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void test_lmmap_get_size(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_get_size <<<<<<<<<\n");
	int size;
	struct mmaphdr a;
	struct mmaphdr b;
	struct mmaphdr c;

	a.prev = NULL;
	a.next = &b;
	a.size = 1;
	b.prev = &a;
	b.next = &c;
	b.size = 2;
	c.prev = &b;
	c.next = NULL;
	c.size = 3;
	assert((size = lmmap_get_size(&a)) == 3);
	printf("Size of list = %d\n", size);

	clear_bins();
	printf("\n>>>>>>>>> END TEST lmmap_get_size <<<<<<<<<\n");
}

void test_lmmap_new(void)
{
	struct mmaphdr *a;

	printf("\n>>>>>>>>> BEGIN TEST lmmap_new <<<<<<<<<\n");
	printf("Creating a linked list of 1 mmap:\n");
	assert(lmmap_new(&a, 1000) != NULL);
	printf("New map of %d elems\n", lmmap_get_size(a));
	printf("Dump of the 4096 bytes of the init mmap area:\n");
	print_mem(a, 4096);
	clear_bins();
	printf(">>>>>>>>>> END TEST lmmap_new <<<<<<<<<<\n");
}

void test_lmmap_push_back(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_push_back <<<<<<<<<\n");
	struct mmaphdr *a;

	printf("Creating a linked list of 5 mmaps:\n");
	assert(lmmap_new(&a, 1000) != NULL);
	assert(lmmap_push_back(a, 10000) != NULL);
	assert(lmmap_push_back(a, 20000) != NULL);
	assert(lmmap_push_back(a, 30000) != NULL);
	assert(lmmap_push_back(a, 40000) != NULL);
	lmmap_print_all(a);
	printf("New map of %d elems\n", lmmap_get_size(a));
	clear_bins();
	printf(">>>>>>>>>> END TEST lmmap_push_back <<<<<<<<<<\n");
}

void test_lmmap_rm_first_elem(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_rm_first_elem <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *b;

	assert(lmmap_new(&a, 1000) != NULL);
	assert((b = lmmap_push_back(a, 10000)) != NULL);
	assert(lmmap_push_back(a, 20000) != NULL);
	lmmap_print_all(a);

	printf("\nlmmap rm first elem from list of %d elems:\n", lmmap_get_size(a));
	assert(lmmap_rm_elem(a) != -1);
	printf("New map of %d elems:\n", lmmap_get_size(b));
	lmmap_print_all(b);
	clear_bins();
	printf(">>>>>>>>>> END TEST lmmap_rm_first_elem <<<<<<<<<<\n");
}

void test_lmmap_rm_middle_elem(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_rm_middle_elem <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *b;

	assert(lmmap_new(&a, 1000) != NULL);
	assert((b = lmmap_push_back(a, 10000)) != NULL);
	assert(lmmap_push_back(a, 20000) != NULL);
	lmmap_print_all(a);

	printf("\nlmmap rm middle elem from list of %d elems:\n", lmmap_get_size(a));
	assert(lmmap_rm_elem(b) != -1);
	printf("New map of %d elems:\n", lmmap_get_size(a));
	lmmap_print_all(a);
	clear_bins();
	printf(">>>>>>>>>> END TEST lmmap_rm_middle_elem <<<<<<<<<<\n");
}

void test_lmmap_rm_last_elem(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_rm_last_elem <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *c;

	assert(lmmap_new(&a, 1000) != NULL);
	assert(lmmap_push_back(a, 10000) != NULL);
	assert((c = lmmap_push_back(a, 20000)) != NULL);
	lmmap_print_all(a);

	printf("\nlmmap rm last elem from list of %d elems:\n", lmmap_get_size(a));
	assert(lmmap_rm_elem(c) != -1);
	printf("New map of %d elems:\n", lmmap_get_size(a));
	lmmap_print_all(a);
	clear_bins();
	printf(">>>>>>>>>> END TEST lmmap_rm_last_elem <<<<<<<<<<\n");
}

void test_lmmap_bestfit_one_mmap_empty(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_one_mmap_empty <<<<<<<<<\n");
	struct mmaphdr *a;
	struct chkhdr *best;

	assert(lmmap_new(&a, 1000) != NULL);
	printf("New lmmap of %d elems:\n", lmmap_get_size(a));

	assert((best = lmmap_bestfit(a, 16)) == a->first_free.next_free);
	printf("Bestfit is a chk with size = %zu (%#lx)\n", best->size, best->size);
	print_mem(a, 4096);

	printf("\n>>>>>>>>> END TEST lmmap_bestfit_one_mmap_empty <<<<<<<<<\n");
}

void test_lmmap_bestfit_one_mmap_full(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_one_mmap_full <<<<<<<<<\n");
	struct mmaphdr *a;

	assert(lmmap_new(&a, 1000) != NULL);
	printf("New lmmap of %d elems:\n", lmmap_get_size(a));

	assert(chk_alloc(a->first_free.next_free, a->first_free.next_free->size) != NULL);
	assert(lmmap_bestfit(a, 16) == NULL);
	printf("No bestfit found\n");
	print_mem(a, 4096);

	printf("\n>>>>>>>>> END TEST lmmap_bestfit_one_mmap_full <<<<<<<<<\n");
}

void test_lmmap_bestfit_two_mmap_all_empty(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_two_mmap_all_empty <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *b;
	struct chkhdr *best;

	assert(lmmap_new(&a, 1000) != NULL);
	assert((b = lmmap_push_back(a, 1000)) != NULL);
	printf("New lmmap of %d elems:\n", lmmap_get_size(a));

	assert((best = lmmap_bestfit(a, 16)) == a->first_free.next_free);

	printf("Bestfit is a chk with size = %zu (%#lx)\n", best->size, best->size);
	printf("Dump of first mmap:\n");
	print_mem(a, 4096);
	printf("\nDump of second mmap:\n");
	print_mem(b, 4096);
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_two_mmap_all_empty <<<<<<<<<\n");
}

void test_lmmap_bestfit_two_mmap_1st_full(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_two_mmap_1st_full <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *b;
	struct chkhdr *best;

	assert(lmmap_new(&a, 1000) != NULL);
	assert((b = lmmap_push_back(a, 1000)) != NULL);
	printf("New lmmap of %d elems:\n", lmmap_get_size(a));

	assert(chk_alloc(a->first_free.next_free, a->first_free.next_free->size) != NULL);
	assert((best = lmmap_bestfit(a, 16)) == b->first_free.next_free);

	printf("Bestfit is a chk with size = %zu (%#lx)\n", best->size, best->size);
	printf("Dump of first mmap:\n");
	print_mem(a, 4096);
	printf("\nDump of second mmap:\n");
	print_mem(b, 4096);

	printf("\n>>>>>>>>> END TEST lmmap_bestfit_two_mmap_1st_full <<<<<<<<<\n");
}

void test_lmmap_bestfit_two_mmap_all_full(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_two_mmap_all_full <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *b;
	struct chkhdr *best;

	assert(lmmap_new(&a, 1000) != NULL);
	assert((b = lmmap_push_back(a, 1000)) != NULL);
	printf("New lmmap of %d elems:\n", lmmap_get_size(a));

	assert(chk_alloc(a->first_free.next_free, a->first_free.next_free->size) != NULL);
	assert(chk_alloc(b->first_free.next_free, b->first_free.next_free->size) != NULL);
	assert((best = lmmap_bestfit(a, 16)) == NULL);

	printf("no bestfit found\n");
	printf("Dump of first mmap:\n");
	print_mem(a, 4096);
	printf("\nDump of second mmap:\n");
	print_mem(b, 4096);

	printf("\n>>>>>>>>> END TEST lmmap_bestfit_two_mmap_all_full <<<<<<<<<\n");
}

static int comp_size(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

void test_lmmap_bestfit_several_chunks(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_bestfit_several_chunks <<<<<<<<<\n");
	struct mmaphdr *a;
	struct mmaphdr *b;

	assert(lmmap_new(&a, 1000) != NULL);
	assert((b = lmmap_push_back(a, 1000)) != NULL);
	printf("New lmmap of %d elems:\n", lmmap_get_size(a));

	struct chkhdr * a_free[10] = { NULL };
	struct chkhdr * b_free[10] = { NULL };

	int i = 0;
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 16)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 128)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 64)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 512)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 16)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 80)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 2048)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 96)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 32)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);
	assert((a_free[i++] = chk_alloc(a->first_free.next_free, 48)) != NULL);
	assert(chk_alloc(a->first_free.next_free, 16) != NULL);

	i = 0;
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, 96)) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, 128)) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, 112)) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, chk_size_16align(200))) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, chk_size_16align(150))) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, 80)) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, chk_size_16align(600))) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, chk_size_16align(180))) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, 32)) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);
	assert((b_free[i++] = chk_alloc(b->first_free.next_free, 320)) != NULL);
	assert(chk_alloc(b->first_free.next_free, 16) != NULL);

	printf("successfully done all the allocation\n");

	size_t all_sizes[20];
	for (int j = 0; j < 10; ++j) {
		all_sizes[j] = a_free[j]->size;
		assert(chk_free(a_free[j], a->first_chk, a->last_chk, &a->first_free) != NULL);
	}
	for (int j = 0; j < 10; ++j) {
		all_sizes[j + 10] = b_free[j]->size;
		assert(chk_free(b_free[j], b->first_chk, b->last_chk, &b->first_free) != NULL);
	}

	printf("Dump of first mmap:\n");
	print_mem(a, 4096);
	printf("\nDump of second mmap:\n");
	print_mem(b, 4096);

	struct chkhdr *tmp = a->first_free.next_free;
	printf("\nPrinting chk free sizes for 1st mmap:\n");
	while (tmp) {
		printf("chk free size = %zu (%#zx)\n", tmp->size, tmp->size);
		tmp = tmp->next_free;
	}

	printf("Printing chk free sizes for 1st mmap:\n");
	tmp = b->first_free.next_free;
	while (tmp) {
		printf("chk free size = %zu (%#zx)\n", tmp->size, tmp->size);
		tmp = tmp->next_free;
	}

	printf("Sorted list of free chunk sizes:\n");
	qsort(all_sizes, 20, sizeof(*all_sizes), comp_size);
	for (int j = 0; j < 20; ++j)
		printf("%zu ", all_sizes[j]);

	printf("\n\nTesting bestfit\n");
	for (size_t size = 16; size < 4096 + 32; size += 16) {
		tmp = lmmap_bestfit(a, size);
		if (!tmp)
			printf("bestfit for size=%zu not found\n", size);
		else
			printf("bestfit for size=%zu is %zu\n", size, (size_t)tmp->size);
	}

	printf("\n>>>>>>>>> END TEST lmmap_bestfit_several_chunks <<<<<<<<<\n");

}