/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:47:04 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 19:41:29 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

#include "../src/lmmap.h"
#include "../src/utils.h"

static void output_nb(const char *s, size_t nb)
{
	ft_puts(s);
	ft_putnbr_base(nb, DEC);
	ft_puts("\n");
}

int main(void)
{
	struct rusage usage = {0};

	getrusage(RUSAGE_SELF, &usage);
	output_nb("before first malloc: ", usage.ru_minflt);

	getrusage(RUSAGE_SELF, &usage);
	output_nb("before first malloc (2nd time): ", usage.ru_minflt);

	struct mmaphdr *ptr;
	lmmap_new(&ptr, 8192);

	getrusage(RUSAGE_SELF, &usage);
	output_nb("after lmmap: ", usage.ru_minflt);

	// memset(ptr, 'A', 4096);
	// getrusage(RUSAGE_SELF, &usage);
	// output_nb("after memset first page: ", usage.ru_minflt);

	// memset((uint8_t *)ptr + 4096, 'A', 4096);
	// getrusage(RUSAGE_SELF, &usage);
	// output_nb("after memset second page: ", usage.ru_minflt);

	// memset((uint8_t *)ptr + 8192, 'B', 4000);
	// getrusage(RUSAGE_SELF, &usage);
	// output_nb("after memset third page: ", usage.ru_minflt);


	return 0;
}

