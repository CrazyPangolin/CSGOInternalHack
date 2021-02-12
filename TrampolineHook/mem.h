#pragma once

#include "pch.h"

namespace mem
{
	bool Detour32(char* src, char* dst, const intptr_t len);
	BYTE* TrampHook32(char* src, char* dst, const intptr_t len);
	
}