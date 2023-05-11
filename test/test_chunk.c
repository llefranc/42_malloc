/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 10:34:48 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/11 12:48:01 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/chunk.h"
#include "../src/lmmap.h"

#include <stdio.h>

void test_chk_moves(void)
{
	struct mmaphdr *m = lmmap_new(4000);
	lmmap_print_all(m);

	t_chkhdr *hdr = (t_chkhdr *)(m + 1);
	t_chkhdr *save = hdr;

	printf("%p\n", hdr);
	chk_alloc(hdr, 4);
	hdr = chk_next_hdr(hdr);
	printf("%p\n", hdr);
	chk_alloc(hdr, 2);
	hdr = chk_next_hdr(hdr);
	chk_alloc(hdr, 6);

	while (save->size != 0) {
		printf("%p: is alloc = %d, size = %zu\n", save, save->is_alloc, (size_t)save->size);
		save = chk_htof(save);
		save = chk_ftoh(save);
		save = chk_next_hdr(save);
	}
	save--;
	while (save->size != 0) {
		printf("%p: is alloc = %d, size = %zu\n", save, save->is_alloc, (size_t)save->size);
		save = chk_ftoh(save);
		save = chk_htof(save);
		save = chk_prev_ftr(save);
	}

	print_mem(m, 100);
}