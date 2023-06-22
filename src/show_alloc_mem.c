/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:51 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/22 17:32:10 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "mutex.h"
#include "bins.h"
#include "lmmap.h"
#include "chunk.h"
#include "utils.h"

/**
 * Display info of an allocated chunk (start address, end address and size) and
 * return its size. If the chunk is free, does nothing and return 0.
*/
static inline size_t print_alloc_chk(struct chkhdr *hdr)
{
	if (!chk_isalloc(hdr->info))
		return 0;
	ft_puts("0x");
	ft_putnbr_base((size_t)(((uint8_t*)hdr) + BNDARY_TAG_SIZE), HEX);
	ft_puts(" - 0x");
	ft_putnbr_base((size_t)chk_htof(hdr), HEX);
	ft_puts(" : ");
	ft_putnbr_base(chk_size(hdr->info), DEC);
	ft_puts(" bytes\n");
	return chk_size(hdr->info);
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
		ft_puts(name);
		ft_puts(" : 0x");
		ft_putnbr_base((size_t)(*bin), HEX);
		ft_puts(" (");
		ft_putnbr_base((*bin)->nb_alloc, DEC);
		ft_puts(" alloc)\n");
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
	ft_puts("############### SHOW_ALLOC_MEM ###############\n");
	ft_puts("Allocated memory:\n");
	while (tbin || sbin || lbin) {
		if (tbin && (!sbin || tbin < sbin) && (!lbin || tbin < lbin)) {
			size += print_bin(&tbin, "TINY");
		} else if (sbin && (!lbin || sbin < lbin)) {
			size += print_bin(&sbin, "SMALL");
		} else if (lbin) {
			size += print_bin(&lbin, "LARGE");
		}
	}
	ft_puts("Total : ");
	ft_putnbr_base(size, DEC);
	ft_puts(" bytes\n");
	ft_puts("##############################################\n");
	mutex_unlock();
}