/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:31 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/09 16:25:01 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
#define TEST_H

#include "../src/chunk.h"

void print_free_chunks(struct chkhdr *first_free);
void print_mem(void *ptr, int nb_bytes);
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
void test_lmmap_rm_first_elem(void);
void test_lmmap_rm_middle_elem(void);
void test_lmmap_rm_last_elem(void);
void test_lmmap_bestfit_one_mmap_empty(void);
void test_lmmap_bestfit_one_mmap_full(void);
void test_lmmap_bestfit_two_mmap_all_empty(void);
void test_lmmap_bestfit_two_mmap_1st_full(void);
void test_lmmap_bestfit_two_mmap_all_full(void);
void test_lmmap_bestfit_several_chunks(void);

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

void test_show_alloc_mem_no_alloc(void);
void test_show_alloc_mem_x_malloc(size_t nb_alloc, size_t start_alloc_size);

#endif /* TEST_H */