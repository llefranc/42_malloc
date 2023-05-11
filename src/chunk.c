/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:02:55 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/11 12:47:34 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"

#include <stdio.h>

void chk_alloc(void *ptr, short size)
{
	t_chkhdr *hdr = ptr;
	t_chkftr *ftr;

	printf("hdr is_alloc = %d, hdr size = %zu\n", hdr->is_alloc, (size_t)hdr->size);

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