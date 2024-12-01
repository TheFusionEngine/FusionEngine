#pragma once

#include "servers/visual_server.h"

#include "gu_common.h"

#include "buddy_alloc.h"

// #define BUFFER_SIZE (2) //for half
#define BUFFER_SIZE (4)

#define FBP0_OFFSET (0)
#define FBP1_OFFSET (FBP0_OFFSET + (512 * 272 * BUFFER_SIZE))
#define FBP2_OFFSET (FBP1_OFFSET + (512 * 272 * BUFFER_SIZE))

#define EDRAM_OFFSET (FBP2_OFFSET + (512 * 272 * 2))

struct MemoryPoolEDRAM final {
	static _FORCE_INLINE_ MemoryPoolEDRAM *get_singleton() { return m_singleton; }

	void *alloc(size_t p_sz);
	void *realloc(void *p_ptr, size_t p_sz);
	void free(void *p_ptr);

	MemoryPoolEDRAM();
	~MemoryPoolEDRAM();

private:
	static MemoryPoolEDRAM *m_singleton;

	uint8_t *m_meta;
	buddy *m_buddy;
};

_FORCE_INLINE_ void *edalloc(size_t p_sz) { return MemoryPoolEDRAM::get_singleton()->alloc(p_sz); }

_FORCE_INLINE_ void edfree(void *p_mem) { MemoryPoolEDRAM::get_singleton()->free(p_mem); }

_FORCE_INLINE_ void *gualloc(size_t p_sz) {
	void *mem = edalloc(p_sz);
	if (mem) {
		// const auto *edstart = sceGeEdramGetAddr() + EDRAM_OFFSET;
		// const auto *edend = edstart + sceGeEdramGetSize() - EDRAM_OFFSET;
		// ERR_FAIL_COND_V(mem < edstart || mem >= edend, nullptr);
		return mem;
	}
	mem = memalloc(p_sz);
	if (mem)
		return mem;
	ERR_FAIL_V(nullptr);
}

_FORCE_INLINE_ void gufree(void *p_ptr) {
	const auto *edstart = sceGeEdramGetAddr();
	const auto *edend = edstart + sceGeEdramGetSize();
	if (p_ptr >= edstart && p_ptr < edend) {
		edfree(p_ptr);
	} else {
		memfree(p_ptr);
	}
}
