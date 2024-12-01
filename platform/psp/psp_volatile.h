#pragma once

#include "servers/visual_server.h"

#include "gu_common.h"

#include "buddy_alloc.h"

#include "psp_edram.h"

struct MemoryPoolPSPVolatile final {
	static _FORCE_INLINE_ MemoryPoolPSPVolatile *get_singleton() {
        return m_singleton;
    }

	void *alloc(size_t p_sz);
	void *realloc(void *p_ptr, size_t p_sz);
	void free(void *p_ptr);

	MemoryPoolPSPVolatile();
	~MemoryPoolPSPVolatile();

    _FORCE_INLINE_ int size() { return m_size; }

    _FORCE_INLINE_ void *offset() { return m_pointer; }

private:
	static MemoryPoolPSPVolatile *m_singleton;

    int m_size;
    void *m_pointer;

	uint8_t *m_meta;
	buddy *m_buddy;
};

_FORCE_INLINE_ void *vmalloc(size_t p_sz) { return MemoryPoolPSPVolatile::get_singleton()->alloc(p_sz); }

_FORCE_INLINE_ void vmfree(void *p_mem) { MemoryPoolPSPVolatile::get_singleton()->free(p_mem); }

_FORCE_INLINE_ void *vtalloc(size_t p_sz) {
	void *mem = vmalloc(p_sz);
	if (mem)
		return mem;
	mem = gualloc(p_sz);
	if (mem)
		return mem;
	ERR_FAIL_V(nullptr);
}

_FORCE_INLINE_ void vtfree(void *p_ptr) {
	const auto *vmstart = MemoryPoolPSPVolatile::get_singleton()->offset();
	const auto *vmend = vmstart + MemoryPoolPSPVolatile::get_singleton()->size();
	if (p_ptr >= vmstart && p_ptr < vmend) {
		vmfree(p_ptr);
	} else {
		gufree(p_ptr);
	}
}