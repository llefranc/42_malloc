/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/01 18:58:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"
#include "../src/bins.h"
#include "../src/lmmap.h"
#include "../src/chunk.h"

#include "test.h"

int main(void)
{	
	// test_lmmap_get_size();
	// test_lmmap_new();
	// test_lmmap_push_back();
	// test_lmmap_rm_first_elem();
	// test_lmmap_rm_middle_elem();
	// test_lmmap_rm_last_elem();

	// test_chk_is_alloc_ok();
	// test_chk_alloc();
	// test_chk_moves();
	// test_chk_free_basics();
	// test_chk_free_no_merge_one_big_chunk();
	// test_chk_free_no_merge_several_small_chunks();
	// test_chk_free_merge_prev();
	// test_chk_free_merge_next();
	// test_chk_free_merge_prev_and_next();
	// test_chk_free_all_with_all_cases();

	// test_malloc_large_one_alloc_one_page();
	// test_malloc_large_one_alloc_multi_pages();
	// test_malloc_large_one_exact_page_size();
	// test_malloc_large_several_allocs();
	// test_malloc_large_n_allocs_with_n_munmaps(100000);

	// test_show_alloc_mem_no_alloc();
	// test_show_alloc_mem_x_malloc(1, 2052);
	// test_show_alloc_mem_x_malloc(5, 2052);
	// test_show_alloc_mem_x_malloc(10, 2052);
	// test_show_alloc_mem_x_malloc(10, 20000);

	return 0;
}
