/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lmmap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:22 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/10 12:41:01 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/lmmap.h"

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


void test_lmmap_get_size(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_get_size <<<<<<<<<\n");
	int size;
	struct mmap_info a;
	struct mmap_info b;
	struct mmap_info c;

	a.prev = NULL;
	a.next = &b;
	a.size = 1;
	b.prev = &a;
	b.next = &c;
	b.size = 2;
	c.prev = &b;
	c.next = NULL;
	c.size = 3;
	assert((size = lmmap_get_size(&a)) == 3);
	printf("Size of list = %d\n", size);
	printf("\n>>>>>>>>> END TEST lmmap_get_size <<<<<<<<<\n");
}

void test_lmmap_new(void)
{
	struct mmap_info *a;

	printf("\n>>>>>>>>> BEGIN TEST lmmap_new <<<<<<<<<\n");
	printf("Creating a linked list of 1 mmap:\n");
	assert((a = lmmap_new(1000)) != NULL);
	printf("New map of %d elems\n", lmmap_get_size(a));
	printf(">>>>>>>>>> END TEST lmmap_new <<<<<<<<<<\n");
}

void test_lmmap_push_back(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_push_back <<<<<<<<<\n");
	struct mmap_info *a;

	printf("Creating a linked list of 5 mmaps:\n");
	assert((a = lmmap_new(1000)) != NULL);
	assert(lmmap_push_back(a, 10000) != NULL);
	assert(lmmap_push_back(a, 20000) != NULL);
	assert(lmmap_push_back(a, 30000) != NULL);
	assert(lmmap_push_back(a, 40000) != NULL);
	lmmap_print(a);
	printf("New map of %d elems\n", lmmap_get_size(a));
	printf(">>>>>>>>>> END TEST lmmap_push_back <<<<<<<<<<\n");
}

void test_lmmap_rm_first_elem(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_rm_first_elem <<<<<<<<<\n");
	struct mmap_info *a;
	struct mmap_info *b;

	assert((a = lmmap_new(1000)) != NULL);
	assert((b = lmmap_push_back(a, 10000)) != NULL);
	assert(lmmap_push_back(a, 20000) != NULL);
	lmmap_print(a);

	printf("\nlmmap rm first elem from list of %d elems:\n", lmmap_get_size(a));
	assert(lmmap_rm_elem(a) != -1);
	printf("New map of %d elems:\n", lmmap_get_size(b));
	lmmap_print(b);
	printf(">>>>>>>>>> END TEST lmmap_rm_first_elem <<<<<<<<<<\n");
}

void test_lmmap_rm_middle_elem(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_rm_middle_elem <<<<<<<<<\n");
	struct mmap_info *a;
	struct mmap_info *b;

	assert((a = lmmap_new(1000)) != NULL);
	assert((b = lmmap_push_back(a, 10000)) != NULL);
	assert(lmmap_push_back(a, 20000) != NULL);
	lmmap_print(a);

	printf("\nlmmap rm middle elem from list of %d elems:\n", lmmap_get_size(a));
	assert(lmmap_rm_elem(b) != -1);
	printf("New map of %d elems:\n", lmmap_get_size(a));
	lmmap_print(a);
	printf(">>>>>>>>>> END TEST lmmap_rm_middle_elem <<<<<<<<<<\n");
}

void test_lmmap_rm_last_elem(void)
{
	printf("\n>>>>>>>>> BEGIN TEST lmmap_rm_last_elem <<<<<<<<<\n");
	struct mmap_info *a;
	struct mmap_info *c;

	assert((a = lmmap_new(1000)) != NULL);
	assert(lmmap_push_back(a, 10000) != NULL);
	assert((c = lmmap_push_back(a, 20000)) != NULL);
	lmmap_print(a);

	printf("\nlmmap rm last elem from list of %d elems:\n", lmmap_get_size(a));
	assert(lmmap_rm_elem(c) != -1);
	printf("New map of %d elems:\n", lmmap_get_size(a));
	lmmap_print(a);
	printf(">>>>>>>>>> END TEST lmmap_rm_last_elem <<<<<<<<<<\n");
}