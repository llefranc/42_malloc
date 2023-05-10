/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lmmap.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/10 17:17:46 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LMMAP_H
#define LMMAP_H

#include <stddef.h>

struct mmaphdr {
	void *prev;
	void *next;
	size_t size;
	int nb_alloc;
};

int lmmap_get_size(struct mmaphdr *ptr);
void * lmmap_new(size_t size);
void * lmmap_push_back(struct mmaphdr *ptr, size_t size);
int lmmap_rm_elem(struct mmaphdr *elem);
void lmmap_print_elem(struct mmaphdr *elem);
void lmmap_print_all(struct mmaphdr *ptr);

#endif /* LMMAP_H */