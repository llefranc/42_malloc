/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:31 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/20 17:28:32 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
#define TEST_H

#include "../src/chunk.h"

void print_free_chunks(struct chkhdr *first_free);
void show_alloc_mem_ex(void *ptr, int nb_bytes);
void clear_bins(void);

void test_chk_is_alloc_ok(void);
void test_chk_size_16align(void);
void test_chk_alloc(void);
void test_chk_moves(void);
void test_chk_free_basics(void);
void test_chk_free_no_merge_one_big_chunk(void);
void test_chk_free_no_merge_several_small_chunks(void);
void test_chk_free_merge_prev(void);
void test_chk_free_merge_next(void);
void test_chk_free_merge_prev_and_next(void);
void test_chk_free_all_with_all_cases(void);

void test_lmmap_get_size(void);
void test_lmmap_new(void);
void test_lmmap_push_back(void);
void test_lmmap_rm_one_elem(void);
void test_lmmap_rm_first_elem(void);
void test_lmmap_rm_middle_elem(void);
void test_lmmap_rm_last_elem(void);
void test_lmmap_bestfit_one_mmap_empty(void);
void test_lmmap_bestfit_one_mmap_full(void);
void test_lmmap_bestfit_two_mmap_all_empty(void);
void test_lmmap_bestfit_two_mmap_1st_full(void);
void test_lmmap_bestfit_two_mmap_all_full(void);
void test_lmmap_bestfit_several_chunks(void);
void test_lmmap_bestfit_smallest_fit_possible(void);
void test_lmmap_get_elem(void);

void test_malloc_tiny_one_alloc_min_size(void);
void test_malloc_tiny_one_alloc_max_size(void);
void test_malloc_tiny_fill_one_mmap(void);
void test_malloc_tiny_all_values_from_tiny_range(void);

void test_malloc_small_one_alloc_min_size(void);
void test_malloc_small_one_alloc_max_size(void);
void test_malloc_small_fill_one_mmap(void);
void test_malloc_small_all_values_from_small_range(void);

void test_malloc_large_one_alloc_one_page(void);
void test_malloc_large_one_alloc_multi_pages(void);
void test_malloc_large_one_exact_page_size(void);
void test_malloc_large_several_allocs(void);
void test_malloc_large_n_allocs_with_n_munmaps(size_t nb_mallocs);

void test_malloc_zero(void);
void test_malloc_with_size(size_t size);
void test_malloc_n_allocs_with_n_munmaps(size_t nb_mallocs, size_t alloc_size);

void test_free_one_tiny(void);
void test_free_all_tiny_range(void);
void test_free_one_small(void);
void test_free_all_small_range(void);
void test_free_one_large(void);
void test_free_several_large(void);
void test_free_empty_bins(void);

void test_realloc_ptr_null(void);
void test_realloc_size_0(void);
void test_realloc_inc_not_in_same_bin(void);
void test_realloc_dec_not_in_same_bin(void);
void test_realloc_same_size(void);
void test_realloc_dec_but_not_enough_space_to_split(void);
void test_realloc_dec_split_chunk(void);
void test_realloc_dec_split_chunk_merge(void);
void test_realloc_dec_then_inc_same_bin(void);
void test_realloc_dec_split_chunk_second_bin(void);
void test_realloc_dec_in_same_page_does_nothing(void);
void test_realloc_dec_trim_pages(void);
void test_realloc_inc_first_bin_range_all_values(void);
void test_realloc_inc_second_bin_range_all_values(void);
void test_realloc_dec_first_bin_range_all_values(void);
void test_realloc_dec_second_bin_range_all_values(void);
void realloc_test_limits_between_pages(void);

void test_show_alloc_mem_no_alloc(void);
void test_show_alloc_mem_x_malloc(size_t nb_alloc, size_t start_alloc_size);
void test_show_alloc_mem_empty_bin_in_the_middle(void);

#endif /* TEST_H */