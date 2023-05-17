/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:02:55 by llefranc          #+#    #+#             */
/*   Updated: 2023/05/17 20:23:23 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"

#include <stdio.h>

/**
 * Round up a size to the closest multiple of 16.
*/
static inline size_t get_16_aligned_size(size_t size)
{
	while (size % 16 != 0)
		++size;
	return size;
}

/**
 * Take a size and return the corresponding allocating size (size value rounded
 * up to the closest multiple of 16 to align it + size of header +
 * size of footer).
*/
size_t chk_get_alloc_size(size_t size)
{
	return get_16_aligned_size(size) + BNDARY_TAG_SIZE * 2;
}

/**
 * Remove a free chunk from the list of free chunks.
*/
static void free_chunk_list_rm(struct chkhdr *free_chk)
{
	if (free_chk->prev_free)
		free_chk->prev_free->next_free = free_chk->next_free;
	if (free_chk->next_free)
		free_chk->next_free->prev_free = free_chk->prev_free;
}

/**
 * Insert between two free chunks a new free chunk.
*/
static void free_chunk_list_insert(struct chkhdr *new_fchk,
				   struct chkhdr *prev_fchk,
				   struct chkhdr *next_fchk)
{
	new_fchk->prev_free = prev_fchk;
	new_fchk->next_free = next_fchk;
	if (prev_fchk)
		prev_fchk->next_free = new_fchk;
	if (next_fchk)
		next_fchk->prev_free = new_fchk;
}

/**
 * Update header and footer of a chunk.
*/
static void update_bnd_tags(struct chkhdr *hdr, struct chkftr *ftr,
                            _Bool is_alloc, size_t size)
{
	hdr->is_alloc = is_alloc;
	hdr->size = size;
	ftr->is_alloc = is_alloc;
	ftr->size = size;
}

// to remove
static void test_fill_data(struct chkhdr *hdr, struct chkftr *ftr)
{
	static char c = 'A';
	uint8_t *ptr = ((uint8_t *)hdr) + BNDARY_TAG_SIZE;
	uint8_t *end = (uint8_t *)ftr;

	++c;
	while (ptr != end) {
		*ptr = c;
		++ptr;
	}
}

/**
 * Print header/footer info of a chunk, and the dump of it's body.
*/
void chk_print(struct chkhdr *hdr)
{
uint8_t *tmp = (uint8_t *)hdr + BNDARY_TAG_SIZE;
	struct chkftr *ftr = chk_htof(hdr);

	printf("---- printing chunk ----\n");
	printf("header: is_alloc=%d, size=%zu\n", hdr->is_alloc, (size_t)hdr->size);
	if (!hdr->is_alloc) {
		printf("prev free chunk addr=%p\n", hdr->prev_free);
		printf("next free chunk addr=%p\n", hdr->next_free);
	}
	printf("footer: is_alloc=%d, size=%zu\n", ftr->is_alloc, (size_t)ftr->size);
	printf("dump of chunk body:");
	for (size_t i = 0; i < hdr->size; ++i) {
		if (i % 16 == 0)
			printf("\n0x%04zu: ", i);
		else if (i % 8 == 0)
			printf("  ");
		printf("%02x ", *tmp);
		++tmp;
	}
	printf("\n------------------------\n");
}

/**
 * Create if possible an allocated chunk inside a free chunk. If the free chunk
 * does the same size than the requested chunk, it will be converted to an
 * allocated chunk. If the free chunk is taller, it will be split in two
 * chunks, one allocated and one freed.
 *
 * @hdr: Header of a free chunk.
 * @size_alloc: The allocated size requested (must be align on 16, and must not
 *              contain the size of boundary tags).
 * Return: A pointer to the header of the newly allocated chunk, NULL if the
 *         allocation is not possible (the targeted chunk is too small or is
 *         already allocated).
*/
struct chkhdr * chk_alloc(struct chkhdr *hdr, size_t size_alloc)
{
	struct chkftr *ftr = chk_htof(hdr);
	struct chkhdr *new_hdr;
	struct chkftr *new_ftr;
	size_t size_free = hdr->size - size_alloc - (2 * BNDARY_TAG_SIZE);

	if (!chk_is_alloc_ok(hdr, size_alloc))
		return NULL;
	if (hdr->size == size_alloc) {
		free_chunk_list_rm(hdr);
		hdr->is_alloc = 1;
		ftr->is_alloc = 1;
		test_fill_data(hdr, ftr); // to remove
	} else {
		/* Updating new alloc chunk */
		new_ftr = (struct chkftr *)((uint8_t *)hdr + BNDARY_TAG_SIZE +
			  size_alloc);
		update_bnd_tags(hdr, new_ftr, 1, size_alloc);

		/* Updating new free chunk */
		new_hdr = (struct chkhdr *)((uint8_t *)ftr - BNDARY_TAG_SIZE -
			  size_free);
		update_bnd_tags(new_hdr, ftr, 0, size_free);

		free_chunk_list_insert(new_hdr, hdr->prev_free, hdr->next_free);
		test_fill_data(hdr, new_ftr); // to remove
	}
	return hdr;
}