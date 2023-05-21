/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:31 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/21 19:45:21 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
#define TEST_H

void print_mem(void *ptr, int nb_bytes);

void test_lmmap_get_size(void);
void test_lmmap_new(void);
void test_lmmap_push_back(void);
void test_lmmap_rm_first_elem(void);
void test_lmmap_rm_middle_elem(void);
void test_lmmap_rm_last_elem(void);

void test_chk_get_alloc_size(void);
void test_chk_is_alloc_ok(void);
void test_chk_alloc(void);
void test_chk_moves(void);
void test_chk_free_basics(void);
void test_chk_free_no_merge_one_big_chunk(void);
void test_chk_free_no_merge_several_small_chunks(void);
void test_chk_free_merge_prev(void);
void test_chk_free_merge_next(void);
void test_chk_free_merge_prev_and_next(void);
void test_chk_free_all_with_all_cases(void);

#endif /* TEST_H */