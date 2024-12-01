#include "psp_volatile.h"

#include "servers/visual_server.h"

#include <pspkernel.h>
#include <psppower.h>
#include <pspsuspend.h>

MemoryPoolPSPVolatile *MemoryPoolPSPVolatile::m_singleton{nullptr};
 
MemoryPoolPSPVolatile::MemoryPoolPSPVolatile()
    : m_pointer{nullptr}, m_size{0}, m_meta{nullptr}, m_buddy{nullptr} {
	m_singleton = this;

	if (!sceKernelVolatileMemLock(0, &m_pointer, &m_size)) {
        scePowerLock(0);

        m_meta = memnew_arr(uint8_t, buddy_sizeof_alignment(m_size, 16));
        m_buddy = buddy_init_alignment(m_meta, reinterpret_cast<uint8_t *>(m_pointer), m_size, 16);
    } else {
        ERR_PRINT("Volatile memory allocation failed");
    }
}

MemoryPoolPSPVolatile::~MemoryPoolPSPVolatile() {
	memfree(m_meta);
}


void *MemoryPoolPSPVolatile::alloc(size_t p_sz) {
    ERR_FAIL_NULL_V(m_buddy, nullptr);
	return buddy_malloc(m_buddy, p_sz);
}

void *MemoryPoolPSPVolatile::realloc(void *p_ptr, size_t p_sz) {
    ERR_FAIL_NULL_V(m_buddy, nullptr);
	return buddy_realloc(m_buddy, p_ptr, p_sz, false);
}

void MemoryPoolPSPVolatile::free(void *p_ptr) {
    ERR_FAIL_NULL(m_buddy);
	return buddy_free(m_buddy, p_ptr);
}