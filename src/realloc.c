/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:55 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/20 16:37:38 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "chunk.h"
#include "bins.h"

#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

/**
 * Check if two sizes belong to the same bin.
*/
static _Bool is_in_same_bin(size_t a, size_t b)
{
	return ((a <= TINY_MAX_ALLOC_SIZE && b <= TINY_MAX_ALLOC_SIZE) ||
		((a > TINY_MAX_ALLOC_SIZE && b > TINY_MAX_ALLOC_SIZE) &&
		(a <= SMALL_MAX_ALLOC_SIZE && b <= SMALL_MAX_ALLOC_SIZE)) ||
		(a > SMALL_MAX_ALLOC_SIZE && b > SMALL_MAX_ALLOC_SIZE));
}

/**
 * Return the corresponding bin of a chunk based on its size and its address.
*/
struct mmaphdr * get_bin(void *alloc_addr, size_t alloc_size)
{
	struct mmaphdr *head;

	if (alloc_size <= TINY_MAX_ALLOC_SIZE)
		head = bins.tiny;
	else if (alloc_size <= SMALL_MAX_ALLOC_SIZE)
		head = bins.small;
	else
		head = bins.large;
	return lmmap_get_elem(head, alloc_addr);
}

/**
 * Decrement the size of an allocated chunk by splitting it into an allocated
 * chunk with the new requested size and a freed chunk just after it.
*/
static void split_chk(struct mmaphdr *bin, struct chkhdr *hdr,
		      size_t size_alloc)
{
	struct chkftr *ftr = chk_htof(hdr);
	struct chkhdr *new_hdr;
	struct chkftr *new_ftr;
	size_t size_free = hdr->size - size_alloc - (2 * BNDARY_TAG_SIZE);

	/* New allocated chunk */
	hdr->size = size_alloc;
	new_ftr = chk_htof(hdr);
	new_ftr->is_alloc = 1;
	new_ftr->size = size_alloc;

	/*
	 * Setting the second chunk resulting of the split to alloc chunk in
	 * order to call chk_free() function on it.
	 */
	ftr->size = size_free;
	new_hdr = chk_ftoh(ftr);
	new_hdr->is_alloc = 1;
	new_hdr->size = size_free;
	chk_free(new_hdr, bin->first_chk, bin->last_chk, &bin->first_free);
}

/**
 * Compare the new allocation size requested with the size of the mmap area
 * for a large allocation, and munmap the usesless pages if the new allocation
 * size requires less memory pages.
*/
static void trim_pages(struct mmaphdr *bin, struct chkhdr *hdr,
		       size_t size_alloc)
{
	/* Only one BNDARY_TAG_SIZE because chk hdr size is in sizeof(*bin) */
	size_t new_size = sizeof(*bin) + size_alloc + BNDARY_TAG_SIZE;
	size_t pagesize = getpagesize();
	struct chkftr *new_ftr;
	void *trim_addr;

	if (new_size % pagesize) {
		new_size += pagesize - (new_size % pagesize);
	}
	if (new_size < bin->size) {
		trim_addr = (uint8_t *)bin + new_size;
		/*
		* BNDARY_TAG_SIZE * 2 for footer + last 8 padding bytes so
		* header and footer can be aligned on 8, and so the pointer
		* returned to the memory allocated can be aligned on 16.
		*/
		hdr->size = new_size - sizeof(*bin) - (BNDARY_TAG_SIZE * 2);
		new_ftr = chk_htof(hdr);
		new_ftr->is_alloc = 1;
		new_ftr->size = hdr->size;

		munmap(trim_addr, bin->size - new_size);
		bin->size = new_size;
		bin->last_chk = new_ftr;
	}
}

void *ft_realloc(void *ptr, size_t size)
{
	struct mmaphdr *bin;
	struct chkhdr *chk;
	size_t size_alloc;
	void *new_alloc;

	if (!ptr) {
		return ft_malloc(size);
	} else if (!size) {
		ft_free(ptr);
		return NULL;
	}
	size_alloc = chk_size_16align(size);
	chk = (struct chkhdr *)((uint8_t *)ptr - BNDARY_TAG_SIZE);

	 // superior, ou nouvelle size n'appartient pas au meme bin, on realloc
	if (size_alloc > chk->size || !is_in_same_bin(size_alloc, chk->size)) {
		if ((new_alloc = ft_malloc(size)) == NULL)
			return NULL;
		if (size > chk->size)
			memcpy(new_alloc, ptr, chk->size);
		else
			memcpy(new_alloc, ptr, size);
		ft_free(ptr);
		return new_alloc;
	}
	// same_size or inferior but can't split. Does nothing. >> a reprendre +
	// peut etre mettre la size pour les larges aussi ?
	if ((size_alloc == chk->size) || !(chk->size >= (size_alloc +
	    sizeof(struct chkftr) + sizeof(struct chkhdr))))
		return ptr;

	// inferior sizes in same bin, large case
	bin = get_bin(ptr, size);
	if (size_alloc > SMALL_MAX_ALLOC_SIZE) {
		trim_pages(bin, chk, size_alloc);
	} else {
		split_chk(bin, chk, size_alloc);
	}
	return ptr;
}