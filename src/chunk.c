/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:02:55 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/08 15:19:53 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chunk.h"

#include <stdio.h>

/**
 * Round up a size to the closest multiple of 16.
*/
size_t chk_size_16align(size_t size)
{
	while (size % 16 != 0)
		++size;
	return size;
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
 * @size_alloc: The allocated size requested. Will be rounded up to the closest
 *              multiple of 16 in order to aligned the chunk.
 * Return: A pointer to the header of the newly allocated chunk, NULL if the
 *         allocation is not possible (the targeted chunk is too small or is
 *         already allocated).
*/
struct chkhdr * chk_alloc(struct chkhdr *hdr, size_t size_alloc)
{
	struct chkftr *ftr = chk_htof(hdr);
	struct chkhdr *new_hdr;
	struct chkftr *new_ftr;
	size_t size_free;

	size_alloc = chk_size_16align(size_alloc);
	size_free = hdr->size - size_alloc - (2 * BNDARY_TAG_SIZE);
	if (!hdr || !chk_is_alloc_ok(hdr, size_alloc))
		return NULL;
	if (hdr->size == size_alloc) {
		free_chunk_list_rm(hdr);
		hdr->is_alloc = 1;
		ftr->is_alloc = 1;
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
	}
	return hdr;
}

/**
 * Merge a free chunk pointed by hdr with its previous neighbour chunk if this
 * one is free by updating headers and footers of the two chunks.
 * As previous chunk is already linked in the free chunked linked list, and the
 * new free chunk is added at its back, there is no need to update the free
 * chunked list.
*/
static int merge_prev_chk(struct chkhdr *hdr, struct chkftr *ftr,
                          struct chkhdr *first_chk)
{
	struct chkhdr *prev_hdr;
	struct chkftr *prev_ftr;
	size_t new_size;

	if (hdr == first_chk)
		return -1;
	prev_ftr = chk_prev_ftr(ftr);
	if (prev_ftr->is_alloc)
		return -1;
	prev_hdr = chk_ftoh(prev_ftr);
	new_size = prev_hdr->size + hdr->size + BNDARY_TAG_SIZE * 2;
	prev_hdr->size = new_size;
	ftr->size = new_size;
	return 0;
}

/**
 * Merge a free chunk pointed by hdr with it next neighbour chunk if this
 * one is free by updating headers and footers of the two chunks.
 * Update also the free chunked linked list by linking the header of the new
 * free chunk, and unlinking the header of the next free chunk since this one
 * is added at the back of the new free chunk.
*/
static int merge_next_chk(struct chkhdr *hdr, struct chkftr *ftr,
                          struct chkftr *last_chk)
{
	struct chkhdr *next_hdr;
	struct chkftr *next_ftr;
	size_t new_size;

	if (ftr == last_chk)
		return -1;
	next_hdr = chk_next_hdr(hdr);
	if (next_hdr->is_alloc)
		return -1;
	next_ftr = chk_htof(next_hdr);
	new_size = hdr->size + next_hdr->size + BNDARY_TAG_SIZE * 2;
	hdr->size = new_size;
	next_ftr->size = new_size;

	hdr->prev_free = next_hdr->prev_free;
	hdr->next_free = next_hdr->next_free;
	if (hdr->prev_free)
		hdr->prev_free->next_free = hdr;
	if (hdr->next_free)
		hdr->next_free->prev_free = hdr;
	return 0;
}

/**
 * Free a chunk, and merge it with its previous/next chunk neighbours if they're
 * free by updating the headers/footers. The free chunked linked list is also
 * updated.
 *
 * @hdr: The header of the chunk to free.
 * @first_chk: The header of the first chunk inside the mmap area (begin limit).
 * @last_chk: The footer of the last chunk inside the mmap area (end limit).
 * @first_free: The head of the free chunked linked list.
 * Return: The header of the new free chunk, or NULL if the chunk is already
 *         free.
*/
struct chkhdr * chk_free(struct chkhdr *hdr, struct chkhdr *first_chk,
                         struct chkftr *last_chk, struct chkhdr *first_free)
{
	struct chkhdr *old_hdr = hdr;
	struct chkftr *ftr = chk_htof(hdr);
	_Bool merge_prev = 0;
	_Bool merge_next = 0;

	if (!hdr || !hdr->is_alloc)
		return NULL;
	hdr->is_alloc = 0;
	ftr->is_alloc = 0;

	/*
	 * Need to merge next chunk before merging previous chunk to correctly
	 * handle the sizes if both merging occur.
	 */
	merge_next = merge_next_chk(hdr, ftr, last_chk) == 0;
	if (merge_next) {
		ftr = chk_htof(hdr);
	}
	merge_prev = merge_prev_chk(hdr, ftr, first_chk) == 0;
	if (merge_prev) {
		hdr = chk_ftoh(ftr);
	}
	if (merge_prev && merge_next) {
		free_chunk_list_rm(old_hdr);
	} else if (!merge_prev && !merge_next) {
		free_chunk_list_insert(hdr, first_free, first_free->next_free);
	}
	return hdr;
}