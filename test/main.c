/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/10 12:39:48 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"
#include "../src/lmmap.h"

#include "test.h"

#include <stdio.h>

int main(void)
{
	test_lmmap_get_size();
	test_lmmap_new();
	test_lmmap_push_back();
	test_lmmap_rm_first_elem();
	test_lmmap_rm_middle_elem();
	test_lmmap_rm_last_elem();
	return 0;
}