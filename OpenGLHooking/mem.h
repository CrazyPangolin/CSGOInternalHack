#pragma once
#include<Windows.h>
namespace mem
{
	void Patch(BYTE* dst, BYTE* src, uintptr_t size);
	void Nop(BYTE* dst, uintptr_t size);
	
}