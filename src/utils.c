/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:13:21 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 18:21:42 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

#include <unistd.h>

size_t ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

void ft_puts(const char *s)
{
	write(1, s, ft_strlen(s));
}

void ft_putnbr_base(size_t n, char *base)
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