/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:12:22 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/04/20 17:15:34 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void* ptr);
void show_alloc_mem(void);

#endif /* ALLOCATOR_H */