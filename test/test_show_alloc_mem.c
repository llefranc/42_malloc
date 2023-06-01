/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_show_alloc_mem.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 21:10:43 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/01 19:00:11 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"

#include "../src/bins.h"
#include "test.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_show_alloc_mem_no_alloc(void)
{
	show_alloc_mem();
}

void test_show_alloc_mem_x_malloc(size_t nb_alloc, size_t start_alloc_size)
{
	printf("\n>>>>>>>>> BEGIN TEST show_alloc_mem_x_malloc <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	size_t alloc_size = start_alloc_size;

	for (size_t i = 0; i < nb_alloc; ++i) {
		printf("allocating size %zu\n", alloc_size);
		assert((ptr = ft_malloc(alloc_size)) != NULL);
		memset(ptr, letter++, alloc_size);
		alloc_size *= 2;		
	}
	show_alloc_mem();
	clear_bins();
	printf("\n>>>>>>>>> END TEST show_alloc_mem_x_malloc <<<<<<<<<\n");
}
