/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:51 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/21 21:34:22 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "bins.h"
#include "lmmap.h"
#include "chunk.h"

#include <stdio.h>

/**
 * Display info of an allocated chunk (start address, end address and size).
*/
static inline size_t print_alloc_chk(struct chkhdr *hdr)
{
	if (!hdr->is_alloc)
		return 0;
	printf("%p - %p : %zu bytes\n", (((uint8_t*)hdr) + BNDARY_TAG_SIZE),
	       chk_htof(hdr), (size_t)hdr->size);
	return hdr->size;
}

/**
 * Display info for all the allocated chunks (start address, end address and
 * size) of a linked list of mmaps area which correspond to a bin.
*/
static size_t print_bin(struct mmaphdr *bin)
{
	struct chkhdr *hdr;
	struct chkhdr *last_chk_hdr;
	size_t size = 0;

	if (!bin)
		return 0;
	last_chk_hdr = chk_ftoh(bin->last_chk);
	while (bin) {
		hdr = bin->first_chk;
		while (hdr != last_chk_hdr) {
			size += print_alloc_chk(hdr);
			hdr = chk_next_hdr(hdr);
		}
		size += print_alloc_chk(hdr);
		bin = bin->next;
	}
	return size;
}

/**
 * Display info of the allocated memory (start address, end address and size)
 * for all the bins.
*/
void show_alloc_mem(void)
{
	size_t size = 0;

	printf("TINY : %p\n", bins.tiny);
	size += print_bin(bins.tiny);
	printf("SMALL : %p\n", bins.small);
	size += print_bin(bins.small);
	printf("LARGE : %p\n", bins.large);
	size += print_bin(bins.large);
	printf("Total : %zu bytes\n", size);
}