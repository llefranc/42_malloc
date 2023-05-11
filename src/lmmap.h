/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lmmap.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/11 13:34:52 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LMMAP_H
#define LMMAP_H

#include "chunk.h"

#include <stddef.h>

struct mmaphdr {
	void *prev;
	void *next;
	size_t size;
	int nb_alloc;
	t_chkhdr empty_hdr; // padding field + indicate start of chunks
	t_chkftr empty_ftr; // padding field + indicate start of chunks
};

/**
 * Return a pointer to the first chunk header, skipping the mmap header and
 * an empty chkftr (to indicate when moving in reverse mode through the chunks
 * that we reach mmap header).
*/
// useless ? need to think about it
static inline t_chkhdr * lmmap_first_chkhdr(struct mmaphdr *ptr)
{
	return (t_chkhdr *)(((uint8_t *)ptr) + sizeof(struct chkinfo));
}

int lmmap_get_size(struct mmaphdr *ptr);
void * lmmap_new(size_t size);
void * lmmap_push_back(struct mmaphdr *ptr, size_t size);
int lmmap_rm_elem(struct mmaphdr *elem);
void lmmap_print_elem(struct mmaphdr *elem);
void lmmap_print_all(struct mmaphdr *ptr);

#endif /* LMMAP_H */