#include <string.h>

#include "Hook.h"
#include "mem.h"

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < 5)
	{
		return false;
	}
	DWORD current_protection_mode;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &current_protection_mode);

	uintptr_t relateive_address = ((uintptr_t)dst - (uintptr_t)src) - 5;

	*src = 0xE9;

	*(uintptr_t*)((uintptr_t)src + 1) = relateive_address;

	DWORD useless_mode;

	VirtualProtect(src, len, current_protection_mode, &useless_mode);

	return true;
}
BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len)
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

	//Write the address of the gateway to the jmp
	*(uintptr_t*)(gateway + len + 1) = gateway_relatetive_address;

	//Perform the detour
	Detour32(src, dst, len);

	return gateway;
}

Hook::Hook(BYTE* src, BYTE* dst, BYTE* ptr_to_gateway_ptr, uintptr_t len)
{
	b_state_ = false;
	this->src_ = src;
	this->dst_ = dst;
	this->len_ = len;
	this->ptr_to_gateway_function_ptr_ = ptr_to_gateway_ptr;
	memset(original_bytes_, 0, 10);
}
Hook::Hook(const char* export_name, const char* mod_name, BYTE* dst, BYTE* ptr_to_gateway_ptr, uintptr_t len)
{
	b_state_ = false;
	HMODULE h_module = GetModuleHandleA(mod_name);
	if (h_module)
	{
		this->src_ = (BYTE*)GetProcAddress(h_module, export_name);
		this->dst_ = dst;
		this->len_ = len;
		this->ptr_to_gateway_function_ptr_ = ptr_to_gateway_ptr;
	}
	memset(original_bytes_, 0, 10);
}


void Hook::Enable()
{
	memcpy(original_bytes_, src_, len_);
	*(uintptr_t*)ptr_to_gateway_function_ptr_ = (uintptr_t)TrampHook32(src_, dst_, len_);
	b_state_ = true;
}

void Hook::Disable()
{
	mem::Patch(src_, original_bytes_, len_);
	b_state_ = false;
}

void Hook::Toggle()
{
	if (!b_state_)
	{
		Enable();
	}
	else
	{
		Disable();
	}
}