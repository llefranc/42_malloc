/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 10:34:48 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/17 18:26:32 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/chunk.h"
#include "../src/lmmap.h"

#include <stdio.h>

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
	if ((tmp = chk_alloc(m->first_free.next_free, 16)) == NULL)
		printf("fatal_err chk_alloc failed\n");
	chk_print(tmp);
	if ((tmp = chk_alloc(m->first_free.next_free, 48)) == NULL)
		printf("fatal_err chk_alloc failed\n");
	chk_print(tmp);
	if ((tmp = chk_alloc(m->first_free.next_free, 32)) == NULL)
		printf("fatal_err chk_alloc failed\n");
	chk_print(tmp);
	if ((tmp = chk_alloc(m->first_free.next_free, 3808)) == NULL)
		printf("fatal_err chk_alloc failed\n");
	// chk_print(tmp);
	if ((tmp = chk_alloc(m->first_free.next_free, 128)) == NULL)
		printf("Alloc should failed because too large\n");
	if ((tmp = chk_alloc(m->first_free.next_free, 56)) == NULL)
		printf("fatal_err chk_alloc failed\n");
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
	if (chk_alloc(m->first_free.next_free, 16) == NULL)
		printf("fatal_err chk_alloc failed\n");
	if (chk_alloc(m->first_free.next_free, 48) == NULL)
		printf("fatal_err chk_alloc failed\n");
	if (chk_alloc(m->first_free.next_free, 32) == NULL)
		printf("fatal_err chk_alloc failed\n");

	struct chkhdr *hdr = (struct chkhdr *)(((uint8_t *)&m->first_free) + sizeof(m->first_free));
	struct chkftr *ftr;
	while (hdr != m->first_free.next_free) {
		chk_print(hdr);
		ftr = chk_htof(hdr);
		hdr = chk_ftoh(ftr);
		hdr = chk_next_hdr(hdr);
	}
	ftr = (struct chkftr *)(((uint8_t *)hdr) - BNDARY_TAG_SIZE);
	printf("changing sides\n");
	while (hdr != (struct chkhdr *)&m->first_chk) {
		hdr = chk_ftoh(ftr);
		chk_print(hdr);
		ftr = chk_htof(hdr);
		ftr = chk_prev_ftr(ftr);
	}
	print_mem(m, 4096);
	printf("\n>>>>>>>>> END TEST chk_moves <<<<<<<<<\n");
}