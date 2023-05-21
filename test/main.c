/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/21 21:12:04 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"
#include "../src/bins.h"
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
		else if (i % 8 == 0)
			printf("  ");
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

	// test_chk_get_alloc_size();
	// test_chk_is_alloc_ok();
	// test_chk_alloc();
	// test_chk_moves();
	// test_chk_free_basics();
	// test_chk_free_no_merge_one_big_chunk();
	// test_chk_free_no_merge_several_small_chunks();
	// test_chk_free_merge_prev();
	// test_chk_free_merge_next();
	// test_chk_free_merge_prev_and_next();
	// test_chk_free_all_with_all_cases();

	test_show_alloc_mem_no_alloc();

	return 0;
}
