/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bins.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:28:19 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 18:33:29 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINS_H
#define BINS_H

#include "lmmap.h"
#include "chunk.h"

#define BIN_MAX_NB_ALLOC (100)

#define TINY_MAX_ALLOC_SIZE (1024)
#define TINY_MAX_CHUNK_SIZE (TINY_MAX_ALLOC_SIZE + 2 * BNDARY_TAG_SIZE)
#define TINY_MMAP_SIZE (sizeof(struct mmaphdr) + (TINY_MAX_CHUNK_SIZE) * \
(BIN_MAX_NB_ALLOC))

#define SMALL_MAX_ALLOC_SIZE (16384)
#define SMALL_MAX_CHUNK_SIZE (SMALL_MAX_ALLOC_SIZE + 2 * BNDARY_TAG_SIZE)
#define SMALL_MMAP_SIZE (sizeof(struct mmaphdr) + (SMALL_MAX_CHUNK_SIZE) * \
(BIN_MAX_NB_ALLOC))

/**
 * Contain the differents bins for tiny, small and large malloc, with each bin
 * being a linked list of mmaps.
 *
 * @tiny: x mmap containing x tiny chunks of memory.
 * @small: x mmap containing x small chunks of memory.
 * @large: x mmap containing each one 1 large allocation.
*/
struct s_bins {
	struct mmaphdr *tiny;
	struct mmaphdr *small;
	struct mmaphdr *large;
};

extern struct s_bins bins;

#endif /* BINS_H */