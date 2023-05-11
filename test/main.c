/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/11 13:30:39 by llefranc         ###   ########.fr       */
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

void print_bins_size(void)
{
	printf("tiny bin size = %zu\n", TINY_MMAP_SIZE);
	printf("small bin size = %zu\n", SMALL_MMAP_SIZE);
}

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
	printf("sizeof chkinfo = %zu\n", sizeof(struct chkinfo));
	// print_bins_size();
	// test_lmmap_get_size();
	// test_lmmap_new();
	// test_lmmap_push_back();
	// test_lmmap_rm_first_elem();
	// test_lmmap_rm_middle_elem();
	// test_lmmap_rm_last_elem();
	test_chk_moves();

	return 0;
}
