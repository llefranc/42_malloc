/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/11 18:00:30 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"
#include "../src/bins.h"
#include "../src/lmmap.h"
#include "../src/chunk.h"

#include "test.h"

int main(void)
{
	// test_chk_is_alloc_ok();
	// test_chk_size_16align();
	// test_chk_alloc();
	// test_chk_moves();
	// test_chk_free_basics();
	// test_chk_free_no_merge_one_big_chunk();
	// test_chk_free_no_merge_several_small_chunks();
	// test_chk_free_merge_prev();
	// test_chk_free_merge_next();
	// test_chk_free_merge_prev_and_next();
	// test_chk_free_all_with_all_cases();

	// test_lmmap_get_size();
	// test_lmmap_new();
	// test_lmmap_push_back();
	test_lmmap_rm_one_elem();
	// test_lmmap_rm_first_elem();
	// test_lmmap_rm_middle_elem();
	// test_lmmap_rm_last_elem();
	// test_lmmap_bestfit_one_mmap_empty();
	// test_lmmap_bestfit_one_mmap_full();
	// test_lmmap_bestfit_two_mmap_all_empty();
	// test_lmmap_bestfit_two_mmap_1st_full();
	// test_lmmap_bestfit_two_mmap_all_full();
	// test_lmmap_bestfit_several_chunks();

	// test_malloc_tiny_one_alloc_min_size();
	// test_malloc_tiny_one_alloc_max_size();
	// test_malloc_tiny_fill_one_mmap();
	// test_malloc_tiny_all_values_from_tiny_range();

	// test_malloc_small_one_alloc_min_size();
	// test_malloc_small_one_alloc_max_size();
	// test_malloc_small_fill_one_mmap();
	// test_malloc_small_all_values_from_small_range();

	// test_malloc_large_one_alloc_one_page();
	// test_malloc_large_one_alloc_multi_pages();
	// test_malloc_large_one_exact_page_size();
	// test_malloc_large_several_allocs();
	// test_malloc_large_n_allocs_with_n_munmaps(100000);

	// test_malloc_zero();

	// test_show_alloc_mem_no_alloc();
	// test_show_alloc_mem_x_malloc(20, 1);
	// test_show_alloc_mem_x_malloc(1, 2052);
	// test_show_alloc_mem_x_malloc(5, 2052);
	// test_show_alloc_mem_x_malloc(10, 2052);
	// test_show_alloc_mem_x_malloc(10, 20000);

	return 0;
}
