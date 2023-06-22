/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 17:31:14 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 19:15:11 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../src/utils.h"
#include "../src/allocator.h"

static void output_nb(const char *s, size_t nb)
{
	ft_puts(s);
	ft_putnbr_base(nb, DEC);
	ft_puts("\n");
}

int main(void)
{
	struct rusage usage = {0};
	int i;
	char *addr;

	getrusage(RUSAGE_SELF, &usage);
	output_nb("before first malloc: ", usage.ru_minflt);

	getrusage(RUSAGE_SELF, &usage);
	output_nb("before first malloc (2nd time): ", usage.ru_minflt);

	i = 0;
	while (i < 6) {
		addr = malloc(1024);
		addr[0] = 'A';
		i++;
		output_nb("allocation nb: ", i);
		getrusage(RUSAGE_SELF, &usage);
		output_nb("after new malloc: ", usage.ru_minflt);

	}

	getrusage(RUSAGE_SELF, &usage);
	output_nb("after all mallocs: ", usage.ru_minflt);

	return 0;
}