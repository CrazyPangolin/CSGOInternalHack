#pragma once

#include "framework.h"

DWORD GetProcId(const wchar_t* proc_name);

uintptr_t GetModuleBaseAddress(DWORD proc_id, const wchar_t* module_name);

uintptr_t FindDMAAddy(HANDLE h_proc, uintptr_t ptr, std::vector<unsigned int> offsets);