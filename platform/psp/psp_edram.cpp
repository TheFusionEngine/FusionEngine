#include "psp_edram.h"

MemoryPoolEDRAM *MemoryPoolEDRAM::m_singleton{nullptr};
 
MemoryPoolEDRAM::MemoryPoolEDRAM() {
	m_singleton = this;

	sceGeEdramSetSize(0x400000);

	const auto m_edramOffset = sceGeEdramGetAddr() + EDRAM_OFFSET;
	const auto m_edramSize = sceGeEdramGetSize() - EDRAM_OFFSET;

	m_meta = memnew_arr(uint8_t, buddy_sizeof_alignment(m_edramSize, 16));
	m_buddy = buddy_init_alignment(m_meta, reinterpret_cast<uint8_t *>(m_edramOffset), m_edramSize, 16);
}

MemoryPoolEDRAM::~MemoryPoolEDRAM() {
	memfree(m_meta);
}


void *MemoryPoolEDRAM::alloc(size_t p_sz) {
	return buddy_malloc(m_buddy, p_sz);
}

void *MemoryPoolEDRAM::realloc(void *p_ptr, size_t p_sz) {
	return buddy_realloc(m_buddy, p_ptr, p_sz, false);
}

void MemoryPoolEDRAM::free(void *p_ptr) {
	return buddy_free(m_buddy, p_ptr);
}