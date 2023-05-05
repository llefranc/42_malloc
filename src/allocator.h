/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:12:22 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/05 16:59:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void *ft_malloc(size_t size);
void *ft_realloc(void *ptr, size_t size);
void ft_free(void* ptr);
void show_alloc_mem(void);

#endif /* ALLOCATOR_H */