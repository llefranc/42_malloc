/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:12:54 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 18:17:19 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define DEC "0123456789"
#define HEX "0123456789abcdef"

size_t ft_strlen(const char *str);
void ft_puts(const char *s);
void ft_putnbr_base(size_t n, char *base);

#endif /* UTILS_H */