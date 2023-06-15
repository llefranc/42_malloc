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

/**
 * Iterate through a mmaps linked list, and if several empty mmaps exist,
 * munmap all of them until only one remain. 
*/
static void free_empty_bins(struct mmaphdr *head)
{
	int nb_empty = 0;
	struct mmaphdr *tmp;
	struct mmaphdr *elem = head;

	while (elem) {
		tmp = elem->next;
		if (!elem->nb_alloc) {
			if (!nb_empty)
				++nb_empty;
			else
				lmmap_rm_elem(&head, elem);
				
		}
		elem = tmp;
	}
}

/**
 * Free an allocated area on the heap.
 * 
 * @ptr: A pointer to an allocated area previously returned by malloc call. Do
 *       nothing if this pointer is NULL.
*/
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
		free_empty_bins(head);
	}
}