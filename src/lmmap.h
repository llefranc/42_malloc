/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lmmap.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/20 17:27:50 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LMMAP_H
#define LMMAP_H

#include "chunk.h"

#include <stddef.h>

/**
 * Header at the beginning of each mmap area, with several info about it.
 * Mmap headers are linked between them. first_chk and last_chk pointers
 * are used to delimitate chunks inside the mmap area.
 *
 * @prev: Pointer to the previous mmap area. NULL if it's the first elem.
 * @next: Pointer to the next mmap area. NULL if it's the last elem.
 * @size: Size of the mmap area (a multiple of page_size).
 * @nb_alloc: Number of allocated chunks inside this mmap.
 * @first_free: Pointer to the first free chunk inside the mmap area (freed
 *              chunks are linked all together starting from this pointer,
 *              which is the head of the linked list).
 * @first_chk: Pointer to the header of the first chunk (corresponds to adress
 *             of start_chk member).
 * @last_chk: Pointer to the footer of the last chunk.
 * @start_chk: Padding field, not used. The address of this member will be the
 *             address of the header of the first chunk in the mmap area. It
 *             allows chunk's headers/footers to be aligned on 8, and the
 *             address returned from ft_malloc call (corresponding to the start
 *             of the chunk's allocated area) to be aligned on 16.
*/
struct mmaphdr {
	void *prev;
	void *next;
	size_t size;
	size_t nb_alloc;
	struct chkhdr first_free;
	struct chkhdr *first_chk;
	struct chkftr *last_chk;
	struct chkhdr *start_chk;
};

int lmmap_get_size(struct mmaphdr *head);
void * lmmap_new(size_t size);
void * lmmap_push_back(struct mmaphdr *head, size_t size);
int lmmap_rm_elem(struct mmaphdr *elem);
void lmmap_print_elem(struct mmaphdr *elem);
void lmmap_print_all(struct mmaphdr *head);

#endif /* LMMAP_H */