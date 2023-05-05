/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:16 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/05 17:00:04 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MMAP_H
#define MMAP_H

#include <stddef.h>

struct mmap_info {
	void *prev;
	void *next;
	// int nb_alloc;
	size_t size;
};

int free_mmap(void *ptr);
void print_mmap_info(struct mmap_info *ptr);
void print_mmap_list(struct mmap_info *ptr);

#endif /* MMAP_H */