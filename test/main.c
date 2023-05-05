/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:42:39 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/05/05 17:01:30 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/allocator.h"

#include "../src/mmap.h"
#include "test.h"

#include <stdio.h>

int main(void)
{
	test_free_mmap();
	return 0;
}