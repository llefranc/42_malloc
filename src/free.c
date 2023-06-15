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
		bin = lmmap_get_elem(head, chk);
		if (chk_free(chk, bin->first_chk, bin->last_chk, 
		             &bin->first_free) != NULL) {
			bin->nb_alloc--;
		}
	}
}