/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:03:44 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/10 19:04:17 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <stddef.h>

/**
 * Information contained in the header/footer of each chunk.
 * @is_alloc: Indicate if the chunk is allocated or not.
 * @size: Indicate the size of the free/allocated chunk. Size is always > 0 for
 *        a free or allocated chunk; if size = 0, then this memory from mmap
 *        was never used by the allocator.
*/
struct chkinfo {
	_Bool is_alloc : 1;
	short size : 15;
};

/**
 * Header of a chunk.
*/
typedef struct chkinfo t_chkhdr;

/**
 * Footer of a chunk.
*/
typedef struct chkinfo t_chkftr;

/**
 * Return a pointer to the chunk header using chunk footer.
*/
static inline t_chkftr * chk_htof(t_chkhdr *hdr)
{
	return (t_chkftr *)((uint8_t *)hdr + sizeof(*hdr) + hdr->size);
}

/**
 * Return a pointer to the chunk footer using the chunk header.
*/
static inline t_chkhdr * chk_ftoh(t_chkftr *ftr)
{
	return (t_chkhdr *)((uint8_t *)ftr - sizeof(*ftr) - ftr->size);
}

/**
 * Return the footer of the chunk before the actual one using its footer.
*/
static inline t_chkftr * chk_prev_ftr(t_chkftr *ftr)
{
	uint8_t *tmp;

	/* No previous chunk */
	if (!ftr->size)
		return NULL;
	tmp = (uint8_t *)ftr;
	tmp -= (sizeof(t_chkftr) + ftr->size + sizeof(t_chkhdr));
	return (t_chkftr *)tmp;
}

/**
 * Return the header of the chunk after the actual one using its header.
*/
static inline t_chkhdr * chk_next_hdr(t_chkhdr *hdr)
{
	uint8_t *tmp;

	/* No next chunk */
	if (!hdr->size)
		return NULL;
	tmp = (uint8_t *)hdr;
	tmp += sizeof(t_chkhdr) + hdr->size + sizeof(t_chkftr);
	return (t_chkhdr *)tmp;
}

void chk_alloc(void *ptr, short size);

void test_chunk(void);

#endif /* CHUNK_H */