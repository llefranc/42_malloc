/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 11:25:23 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"
#include "../src/bins.h"
#include "../src/lmmap.h"
#include "../src/chunk.h"

#include "test.h"

int main(void)
{
	test_chk_is_alloc_ok();
	test_chk_size_16align();
	test_chk_alloc();
	test_chk_moves();
	test_chk_free_basics();
	test_chk_free_no_merge_one_big_chunk();
	test_chk_free_no_merge_several_small_chunks();
	test_chk_free_merge_prev();
	test_chk_free_merge_next();
	test_chk_free_merge_prev_and_next();
	test_chk_free_all_with_all_cases();

	test_lmmap_get_size();
	test_lmmap_new();
	test_lmmap_push_back();
	test_lmmap_rm_one_elem();
	test_lmmap_rm_first_elem();
	test_lmmap_rm_middle_elem();
	test_lmmap_rm_last_elem();
	test_lmmap_bestfit_one_mmap_empty();
	test_lmmap_bestfit_one_mmap_full();
	test_lmmap_bestfit_two_mmap_all_empty();
	test_lmmap_bestfit_two_mmap_1st_full();
	test_lmmap_bestfit_two_mmap_all_full();
	test_lmmap_bestfit_several_chunks();
	test_lmmap_bestfit_smallest_fit_possible();
	test_lmmap_get_elem();

	test_malloc_tiny_one_alloc_min_size();
	test_malloc_tiny_one_alloc_max_size();
	test_malloc_tiny_fill_one_mmap();
	test_malloc_tiny_all_values_from_tiny_range();

	test_malloc_small_one_alloc_min_size();
	test_malloc_small_one_alloc_max_size();
	test_malloc_small_fill_one_mmap();
	test_malloc_small_all_values_from_small_range();

	test_malloc_large_one_alloc_one_page();
	test_malloc_large_one_alloc_multi_pages();
	test_malloc_large_one_exact_page_size();
	test_malloc_large_several_allocs();
	test_malloc_large_n_allocs_with_n_munmaps(100000);

	test_malloc_zero();

	test_free_one_tiny();
	test_free_all_tiny_range();
	test_free_one_small();
	test_free_all_small_range();
	test_free_one_large();
	test_free_several_large();
	test_free_empty_bins();

	test_realloc_ptr_null();
	test_realloc_size_0();
	test_realloc_inc_not_in_same_bin();
	test_realloc_dec_not_in_same_bin();
	test_realloc_same_size();
	test_realloc_dec_but_not_enough_space_to_split();
	test_realloc_dec_split_chunk();
	test_realloc_dec_split_chunk_second_bin();
	test_realloc_dec_split_chunk_merge();
	test_realloc_dec_then_inc_same_bin();
	test_realloc_dec_in_same_page_does_nothing();
	test_realloc_dec_trim_pages();
	test_realloc_inc_first_bin_range_all_values();
	test_realloc_inc_second_bin_range_all_values();
	test_realloc_dec_first_bin_range_all_values();
	test_realloc_dec_second_bin_range_all_values();
	realloc_test_limits_between_pages();

	test_show_alloc_mem_no_alloc();
	test_show_alloc_mem_x_malloc(1, 1);
	test_show_alloc_mem_x_malloc(1, TINY_MAX_ALLOC_SIZE + 1);
	test_show_alloc_mem_x_malloc(1, SMALL_MAX_ALLOC_SIZE + 1);
	test_show_alloc_mem_x_malloc(20, 1);
	test_show_alloc_mem_x_malloc(1, 2052);
	test_show_alloc_mem_x_malloc(5, 2052);
	test_show_alloc_mem_x_malloc(10, 2052);
	test_show_alloc_mem_x_malloc(10, 20000);
	test_show_alloc_mem_empty_bin_in_the_middle();

	return 0;
}
