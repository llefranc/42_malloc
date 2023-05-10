/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lmmap.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/10 12:22:16 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LMMAP_H
#define LMMAP_H

#include <stddef.h>

struct mmap_info {
	void *prev;
	void *next;
	// int nb_alloc;
	size_t size;
};

int lmmap_get_size(void *ptr);
void * lmmap_new(size_t size);
void * lmmap_push_back(void *ptr, size_t size);
int lmmap_rm_elem(void *ptr);
void mmap_print_info(struct mmap_info *ptr);
void lmmap_print(struct mmap_info *ptr);

#endif /* LMMAP_H */