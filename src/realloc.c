/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 12:17:55 by lucaslefran       #+#    #+#             */
/*   Updated: 2023/06/21 12:29:47 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "allocator.h"

#include "mutex.h"
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
static struct mmaphdr * get_bin(void *alloc_addr, size_t alloc_size)
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

/**
 * A new alloc is needed when the aligned allocation size requested is:
 *   - Superior to the actual chunk size available.
 *   - Not a size containable in the actual allocation's bin, in order to keep
 *     all the chunks inside a bin with the same range of sizes to avoid
 *     fragmentation.
 *
 * Ex: alloc_size: 32           chk_size: 12           >>> new alloc
 *     alloc_size: 3000 (LARGE) chk_size: 4000 (LARGE) >>> no new alloc
 *     alloc_size: 32           chk_size: 32           >>> no new alloc
 *     alloc_size: 32 (TINY)    chk_size: 500 (SMALL)  >>> new alloc
*/
static inline _Bool is_new_alloc(size_t size_alloc, size_t chk_size)
{
	return size_alloc > chk_size || !is_in_same_bin(size_alloc, chk_size);
}

/**
 * A decrease of the allocated area occurs when the aligned allocation size
 * requested is smaller than the chunk capacity and that it's possible to
 * split the chunk into an allocated chunk with the new size and a freed
 * chunk (only for SMALL and TINY chunks).
*/
static inline _Bool is_decrease(size_t size_alloc, size_t chk_size)
{
	return chk_size >= (size_alloc + sizeof(struct chkftr) +
	       sizeof(struct chkhdr));
}

/**
 * Reallocate on the heap memory the requested size. Avoid to move the data
 * when it's possible.
 *
 * @ptr: A pointer to a previously allocated area using malloc. If NULL,
 *       then a malloc with size as parameter is performed (even if size=0).
 * @size: The size of the new allocation. If size=0, then a free on ptr is
 *        performed.
 * Return: A pointer to the reallocated area, or NULL with errno set to ENOMEM
 *         if there was not enough memory.
*/
void *realloc(void *ptr, size_t size)
{
	struct mmaphdr *bin;
	struct chkhdr *chk;
	size_t size_alloc;
	void *new_alloc;

	mutex_lock();
	if (!ptr) {
		ptr = malloc(size);
		goto end;
	} else if (!size) {
		free(ptr);
		goto end_null;
	}
	size_alloc = chk_size_16align(size);
	chk = (struct chkhdr *)((uint8_t *)ptr - BNDARY_TAG_SIZE);

	if (is_new_alloc(size_alloc, chk->size)) {
		if ((new_alloc = malloc(size)) == NULL)
			goto end_null;
		if (size > chk->size)
			memcpy(new_alloc, ptr, chk->size);
		else
			memcpy(new_alloc, ptr, size);
		free(ptr);
		ptr = new_alloc;
	} else if (is_decrease(size_alloc, chk->size)) {
		bin = get_bin(ptr, size);
		if (size_alloc > SMALL_MAX_ALLOC_SIZE) {
			trim_pages(bin, chk, size_alloc);
		} else {
			split_chk(bin, chk, size_alloc);
		}
	}
end:
	mutex_unlock();
	return ptr;

end_null:
	mutex_unlock();
	return NULL;
}