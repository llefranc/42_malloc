/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 15:56:17 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/20 17:28:32 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/bins.h"

#include <stdio.h>

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

void show_alloc_mem_ex(void *ptr, int nb_bytes)
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

void print_free_chunks(struct chkhdr *first_free)
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

void clear_bins(void)
{
	assert(lmmap_clear(&bins.tiny) != -1);
	assert(lmmap_clear(&bins.small) != -1);
	assert(lmmap_clear(&bins.large) != -1);
}