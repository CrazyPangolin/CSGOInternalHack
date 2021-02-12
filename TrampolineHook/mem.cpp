#include "pch.h"
#include "mem.h"

bool mem::Detour32(char* src, char* dst, const intptr_t len)
{
	if (len < 5)
	{
		return false;
	}
	DWORD current_protection_mode;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &current_protection_mode);

	uintptr_t relateive_address = ((uintptr_t)dst - (uintptr_t)src) - 5;

	*src = (char)'\xE9';

	*(uintptr_t*)((uintptr_t)src + 1) = relateive_address;

	DWORD useless_mode;
	
	VirtualProtect(src, len, current_protection_mode, &useless_mode);
	
	return true;
}
BYTE* mem::TrampHook32(char* src, char* dst, const intptr_t len)
{
	// Make sure the length is greater than 5
	if (len < 5)
	{
		return nullptr;
	}
	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//Write the stolen bytes into the gateway
	memcpy_s(gateway, len, src, len);
	
	// Add the jmp opcode to the end of the gateway
	*(gateway + len) = 0xE9;

	// Get the gateway to destination addy
	uintptr_t gateway_relatetive_address = ((uintptr_t)src - (uintptr_t)gateway) - 5;
	*(uintptr_t*)(gateway + len + 1) = gateway_relatetive_address;
	
	Detour32(src, dst, len);
	
	return gateway;
}