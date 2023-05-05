/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/05 17:04:19 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/mmap.h"

#include <stdio.h>

void test_free_mmap(void)
{
	printf(">>>>>>>>> BEGIN TEST FREE_MMAP <<<<<<<<<\n");
	struct mmap_info a;
	struct mmap_info b;
	struct mmap_info c;

	a.prev = NULL;
	a.next = &b;
	a.next = &b;
	a.size = 4096;
	b.prev = &a;
	b.next = &c;
	b.next = &c;
	b.size = 16384;
	c.prev = &b;
	c.next = NULL;
	c.size = 512;

	print_mmap_list(&a);
	free_mmap(&b);
	printf("\n\n\n");
	print_mmap_list(&a);

	// print_mmap_list(&a);
	// free_mmap(&a);
	// printf("\n\n\n");
	// print_mmap_list(&b);

	// print_mmap_list(&a);
	// free_mmap(&c);
	// printf("\n\n\n");
	// print_mmap_list(&a);
	printf(">>>>>>>>>> END TEST FREE_MMAP <<<<<<<<<<\n");
}