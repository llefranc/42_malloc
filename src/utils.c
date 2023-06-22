/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:00:46 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/22 16:14:09 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

#include <unistd.h>

void * ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t i;

	i = 0;
	if (dst == NULL && src == NULL)
		return (dst);
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}

size_t ft_strlen(const char *s)
{
	size_t i = 0;

	while (s[i])
		++i;
	return i;
}

void ft_puts(const char *s)
{
	write(1, s, ft_strlen(s));
}

void ft_putnbr_base(size_t n, const char *base)
{
	if (n == 0)
		write(STDOUT_FILENO, "0", 1);
	else
	{
		if (n / ft_strlen(base) != 0)
			ft_putnbr_base(n / ft_strlen(base), base);
		write(STDOUT_FILENO, &base[n % ft_strlen(base)], 1);
	}
}