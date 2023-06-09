/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:39:29 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/09 16:24:46 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

#include "../src/bins.h"
#include "../src/allocator.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/mman.h>

void test_malloc_zero(void)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_zero <<<<<<<<<\n");
	printf("doing an alloc of 0, should failed\n");
	assert(ft_malloc(0) == NULL);
	printf("Success!\n");
	printf("\n>>>>>>>>> END TEST malloc_zero <<<<<<<<<\n");
}

void test_malloc_with_size(size_t size)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_with_size <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;
	printf("allocating a large malloc. Size=%zu\n", size);


	assert((ptr = ft_malloc(size)) != NULL);
	memset(ptr, letter++, size);
	printf("Success!\n");
	printf("\n>>>>>>>>> END TEST malloc_with_size <<<<<<<<<\n");
}

void test_malloc_n_allocs_with_n_munmaps(size_t nb_mallocs, size_t alloc_size)
{
	printf("\n>>>>>>>>> BEGIN TEST malloc_n_allocs_with_n_munmaps <<<<<<<<<\n");
	static char letter = 'A';
	char *ptr;

	printf("doing %zu large mallocs, with each one being immediately munmapped\n", nb_mallocs);
	for (size_t i = 1; i < nb_mallocs; ++i) {
		alloc_size = i;
		printf("doing allocation nb %7zu of size %7zu\n", i, alloc_size);
		// assert((ptr = malloc(alloc_size)) != NULL);
		assert((ptr = ft_malloc(alloc_size)) != NULL);
		memset(ptr, letter++, alloc_size);
		munmap(bins.large, bins.large->size);
		printf("\x1B[%zuA", strlen("doing allocation nb xxxxxxx of size xxxxxxx\n"));
		fflush(stdout);
	}
	printf("successfully did %zu large mallocs\n", nb_mallocs);
	printf("\n>>>>>>>>> END TEST malloc_n_allocs_with_n_munmaps <<<<<<<<<\n");
}