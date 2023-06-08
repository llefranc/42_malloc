/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:03:44 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/08 15:20:27 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>
#include <stddef.h>

/**
 * Information inside the header of each chunk.
 * The two pointers are only used for a free chunk, and are actually not stored
 * in the header of the free chunk but at the beginning of its body. For a free
 * chunk, casting the chunk's address to this struct  allows to manipulate
 * those pointers.
 *
 * @is_alloc: Indicate if the chunk is allocated or not.
 * @size: Indicate the size of the free/allocated chunk.
 * @prev_free: Pointer to the header of the previous free chunk (NULL if first
 *             one).
 * @next_free: Pointer to the header of the next free chunk (NULL if last one).
*/
struct chkhdr {
	size_t size : 63;
	_Bool is_alloc : 1;
	struct chkhdr *prev_free;
	struct chkhdr *next_free;
};

/**
 * Information in the footer of each chunk.
 *
 * @is_alloc: Indicate if the chunk is allocated or not.
 * @size: Indicate the size of the free/allocated chunk.
*/
struct chkftr {
	size_t size : 63;
	_Bool is_alloc : 1;
};

/*
 * Footer does the same size than header, as the 2 pointers inside the header
 * for struct chkhdr are used only for free chunk and are stored inside free
 * chunk body.
 */
#define BNDARY_TAG_SIZE (sizeof(struct chkftr))

/**
 * Return a pointer to the chunk footer using chunk header.
*/
static inline struct chkftr * chk_htof(struct chkhdr *hdr)
{
	return (struct chkftr *)((uint8_t *)hdr + BNDARY_TAG_SIZE + hdr->size);
}

/**
 * Return a pointer to the chunk header using the chunk footer.
*/
static inline struct chkhdr * chk_ftoh(struct chkftr *ftr)
{
	return (struct chkhdr *)((uint8_t *)ftr - BNDARY_TAG_SIZE - ftr->size);
}

/**
 * Take a pointer to a chunk footer and return a pointer to the footer of
 * previous chunk.
*/
static inline struct chkftr * chk_prev_ftr(struct chkftr *ftr)
{
	return (struct chkftr *)(((uint8_t *)ftr) - ftr->size - 2 *
	       BNDARY_TAG_SIZE);
}

/**
 * Take a pointer to a chunk header and return a pointer to the header of
 * next chunk.
*/
static inline struct chkhdr * chk_next_hdr(struct chkhdr *hdr)
{
	return (struct chkhdr *)(((uint8_t *)hdr) + hdr->size + 2 *
	       BNDARY_TAG_SIZE);
}

/**
 * Check if the requested size can exactly fit in the specified free chunk,
 * otherwise check if the free chunk can be splitted into one with the specified
 * size and another chunk with at least a header and a footer.
 *
 * @chk: Header of the free chunk targeted for the allocation.
 * @size: The aligned size of the requested allocation (without the boundary
 *        tags).
 * Return: 1 if chunk is free with enough space, 0 otherwise.
*/
static inline _Bool chk_is_alloc_ok(struct chkhdr *chk, size_t size)
{
	return !chk->is_alloc && (chk->size == size || chk->size >= (size +
	       sizeof(struct chkhdr) + sizeof(struct chkftr)));
}

void chk_print(struct chkhdr *hdr);
size_t chk_size_16align(size_t size);
struct chkhdr * chk_alloc(struct chkhdr *hdr, size_t size_alloc);
struct chkhdr * chk_free(struct chkhdr *hdr, struct chkhdr *first_chk,
                         struct chkftr *last_chk, struct chkhdr *first_free);

#endif /* CHUNK_H */