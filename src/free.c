/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:53 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/04/20 13:01:39 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "bins.h"
#include "lmmap.h"
#include "chunk.h"

static struct mmaphdr * find_bin(struct mmaphdr *head, void *chk_addr)
{
	void *bin_start_addr;
	void *bin_end_addr;

	while (head) {
		bin_start_addr = head;
		bin_end_addr = (void *)((uint8_t *)head + head->size);
		if (chk_addr > bin_start_addr && chk_addr < bin_end_addr)
			break;
		head = head->next;
	}
	return head;
}

// static inline _Bool is_bin_empty(struct mmaphdr *bin)
// {
// 	return bin->first_free.next_free == NULL;
// }

// determiner dans quel bin se trouve le chunk a free grace a la size
// rechopper le mmaphdr
void ft_free(void *ptr)
{
	struct mmaphdr *head;
	struct mmaphdr *bin;
	struct chkhdr *chk;

	if (!ptr)
		return;
	chk = (struct chkhdr *)((uint8_t *)ptr - BNDARY_TAG_SIZE);
	if (chk->size > SMALL_MAX_ALLOC_SIZE) {
		bin = (struct mmaphdr *)((uint8_t *)chk + BNDARY_TAG_SIZE
		      - sizeof(*bin));
		lmmap_rm_elem(&bins.large, bin);
	} else {
		if (chk->size <= TINY_MAX_ALLOC_SIZE) {
			head = bins.tiny;
		} else {
			head = bins.small;
		}
		bin = find_bin(head, chk);
		chk_free(chk, bin->first_chk, bin->last_chk, &bin->first_free);
		// faire fonction pour release les mmap si necessaire
	}
}