/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:12:22 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 18:32:36 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

// #ifdef TEST
// #  define malloc ft_malloc
// #  define free ft_free
// #  define realloc ft_realloc
// #endif

void *ft_malloc(size_t size);
void *ft_realloc(void *ptr, size_t size);
void ft_free(void* ptr);
void show_alloc_mem(void);

#endif /* ALLOCATOR_H */