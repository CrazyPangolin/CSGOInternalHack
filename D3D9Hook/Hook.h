#pragma once

#include <Windows.h>


bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);
BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len);

class Hook
{

public:
	Hook();
	Hook(BYTE* src, BYTE* dst, BYTE* ptr_to_gateway_ptr, uintptr_t len);
	Hook(const char* export_name, const char* mod_name, BYTE* dst, BYTE* ptr_to_gateway_ptr, uintptr_t len);

	void Init(BYTE* src, BYTE* dst, BYTE* ptr_to_gateway_ptr, uintptr_t len);
	void Enable();
	void Disable();
	void Toggle();

private:
	bool b_state_;
	bool b_init_;
	BYTE* src_;
	BYTE* dst_;
	BYTE* ptr_to_gateway_function_ptr_;
	uintptr_t len_;
	BYTE original_bytes_[10];


};

