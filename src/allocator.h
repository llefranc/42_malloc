/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:12:22 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 12:28:28 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef TEST
#  define malloc ft_malloc
#  define free ft_free
#  define realloc ft_realloc
#endif

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void* ptr);
void show_alloc_mem(void);

#endif /* ALLOCATOR_H */