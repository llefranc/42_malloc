/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:02:55 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/10 18:43:08 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"

#include <stdio.h>

void test_chunk(void)
{
	struct chkinfo a;
	printf("size of chunk info : %zu\n", sizeof(a));
}

// void chk_alloc(void *ptr, short size, struct chkinfo *prev)
// {
// 	struct chkhdr *hdr = ptr;

// 	if (!hdr->this.size) {
// 		// empty area

// 		// cas du premier chunk
// 		if (prev == NULL) {
// 			hdr->prev.is_alloc = 0;
// 			hdr->prev.size = 0;
// 		} else {
// 			hdr->prev.is_alloc = prev->is_alloc; // a revoir
// 			hdr->prev.size = prev->size; // a revoir
// 		}
// 		hdr->this.is_alloc = 1;
// 		hdr->this.size = size;
// 		char *c = (char *)(hdr + 1);
// 		for (short i = 0; i < size; ++i) {
// 			*c = 'A';
// 			++c;
// 		}
// 	} else {
// 		// c'est un bloc qui a ete free, il faut peut etre le split
// 	}
// }

void chk_alloc(void *ptr, short size)
{
	t_chkhdr *hdr = ptr;
	t_chkftr *ftr;

	printf("hdr is_alloc = %d, hdr size = %d\n", hdr->is_alloc, hdr->size);

	// empty area
	if (!hdr->size) {
		printf("allocating empty area\n");
		hdr->is_alloc = 1;
		hdr->size = size;
		ftr = chk_htof(hdr);
		ftr->is_alloc = 1;
		ftr->size = size;

		char *c = (char *)(hdr + 1);
		for (short i = 0; i < size; ++i) {
			*c = 'A';
			++c;
		}
	} else {
		printf("free need split\n");
		// c'est un block qui a ete free, il faut peut etre le split
	}
}


// struct chunk chunk_get_next(struct chunk *c)
// {
// 	struct chunk next;

// 	next.hdr = c->ftr + 1;
// 	next.ftr = (struct chkhdr *)(((uint8_t *)next.hdr) + next.hdr->size);
// 	return next;
// }