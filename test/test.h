/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 16:25:31 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/17 20:11:07 by llefranc         ###   ########.fr       */
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

#endif /* TEST_H */