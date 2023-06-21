/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:51 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 11:12:16 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "mutex.h"
#include "bins.h"
#include "lmmap.h"
#include "chunk.h"

#include <stdio.h>

/**
 * Display info of an allocated chunk (start address, end address and size) and
 * return its size. If the chunk is free, does nothing and return 0.
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
 * size) of a mmap area which corresponds to a bin.
*/
static size_t print_bin(struct mmaphdr **bin, const char *name)
{
	struct chkhdr *hdr;
	struct chkhdr *last_chk_hdr;
	size_t size = 0;

	if ((*bin)->nb_alloc) {
		printf("%s : %p (%zu alloc)\n", name, *bin, (*bin)->nb_alloc);
		hdr = (*bin)->first_chk;
		last_chk_hdr = chk_ftoh((*bin)->last_chk);
		while (hdr != last_chk_hdr) {
			size += print_alloc_chk(hdr);
			hdr = chk_next_hdr(hdr);
		}
		size += print_alloc_chk(hdr);
	}
	*bin = (*bin)->next;
	return size;
}

/**
 * Display info of the allocated memory (start address, end address and size)
 * for all the bins, with each bin being displayed in increasing address order.
*/
void show_alloc_mem(void)
{
	size_t size = 0;
	struct mmaphdr *tbin = bins.tiny;
	struct mmaphdr *sbin = bins.small;
	struct mmaphdr *lbin = bins.large;

	mutex_lock();
	printf("############### SHOW_ALLOC_MEM ###############\n");
	printf("Allocated memory:\n");
	while (tbin || sbin || lbin) {
		if (tbin && (!sbin || tbin < sbin) && (!lbin || tbin < lbin)) {
			size += print_bin(&tbin, "TINY");
		} else if (sbin && (!lbin || sbin < lbin)) {
			size += print_bin(&sbin, "SMALL");
		} else if (lbin) {
			size += print_bin(&lbin, "LARGE");
		}
	}
	printf("Total : %zu bytes\n", size);
	printf("##############################################\n");
	mutex_unlock();
}