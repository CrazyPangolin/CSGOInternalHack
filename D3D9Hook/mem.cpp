#include "mem.h"


void mem::Patch(BYTE* dst, BYTE* src, uintptr_t size)
{
	DWORD old_protect_mode;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &old_protect_mode);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, old_protect_mode, &old_protect_mode);
}



void mem::Nop(BYTE* dst, uintptr_t size)
{
	DWORD old_protect_mode;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &old_protect_mode);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, old_protect_mode, &old_protect_mode);
}
