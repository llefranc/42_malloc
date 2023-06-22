/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:58:14 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/22 17:27:06 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define DEC "0123456789"
#define HEX "0123456789abcdef"

void * ft_memcpy(void *dst, const void *src, size_t n);
size_t ft_strlen(const char *s);
void ft_puts(const char *s);
void ft_putnbr_base(size_t n, const char *base);

#endif /* UTILS_H */