/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/10 18:59:09 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"
#include "../src/lmmap.h"
#include "../src/chunk.h"

#include "test.h"

#include <stdio.h>

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <limits.h>

void print_mem(void *ptr, int nb_bytes)
{
	uint8_t *tmp = ptr;

	for (int i = 0; i < nb_bytes; ++i) {
		if (i % 16 == 0)
			printf("\n0x%04x: ", i);
		printf("%02x ", *tmp);
		++tmp;
	}
	printf("\n");
}

int main(void)
{
	// test_lmmap_get_size();
	// test_lmmap_new();
	// test_lmmap_push_back();
	// test_lmmap_rm_first_elem();
	// test_lmmap_rm_middle_elem();
	// test_lmmap_rm_last_elem();

	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	t_chkhdr *hdr = (t_chkhdr *)(m + 1) + 1;
	t_chkhdr *save = hdr;
	(void)save;

	printf("%p\n", hdr);
	chk_alloc(hdr, 4);
	hdr = chk_next_hdr(hdr);
	printf("%p\n", hdr);
	chk_alloc(hdr, 2);
	hdr = chk_next_hdr(hdr);
	chk_alloc(hdr, 6);

	while (save->size != 0) {
		printf("%p: is alloc = %d, size = %d\n", save, save->is_alloc, save->size);
		save = chk_htof(save);
		save = chk_ftoh(save);
		save = chk_next_hdr(save);
	}
	save--;
	while (save->size != 0) {
		printf("%p: is alloc = %d, size = %d\n", save, save->is_alloc, save->size);
		save = chk_ftoh(save);
		save = chk_htof(save);
		save = chk_prev_ftr(save);
	}

	print_mem(m, 100);
	return 0;
}
