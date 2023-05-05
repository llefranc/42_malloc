/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:09:04 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/05 16:24:43 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mmap.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>


/**
 * Clear a mmap by munmaping its memory.
*/
int free_mmap(void *ptr)
{
	struct mmap_info *elem = ptr;
	struct mmap_info *tmp;

	if (elem->prev) {
		tmp = elem->prev;
		tmp->next = elem->next;
	}
	if (elem->next) {
		tmp = elem->next;
		tmp->prev = elem->prev;
	}
	return munmap(elem, elem->size);
}

/**
 * Print infos stored at the beginning of a mmap.
*/
void print_mmap_info(struct mmap_info *ptr)
{
	printf("------ mmap (%p) ------\n", ptr);
	printf("prev mmap addr = %p\n", ptr->prev);
	printf("next mmap addr = %p\n", ptr->next);
	printf("size mmap = %zu\n", ptr->size);
}

/**
 * Print the infos for each mmap of a mmap linked list.
*/
void print_mmap_list(struct mmap_info *ptr)
{
	while (ptr) {
		print_mmap_info(ptr);
		ptr = ptr->next;
	}
}