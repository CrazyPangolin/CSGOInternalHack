#include "pch.h"
#include "process_hack.h"


DWORD GetProcId(const wchar_t* proc_name)
{
	DWORD proc_id = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!h_snap)
	{
		PROCESSENTRY32 proc_entry = { 0 };
		proc_entry.dwSize = sizeof(PROCESSENTRY32);
		if(Process32First(h_snap, &proc_entry))
		{
			do
			{
				if(!_wcsicmp(proc_name, proc_entry.szExeFile))
				{
					proc_id = proc_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(h_snap, &proc_entry));
		}
		CloseHandle(h_snap);
	}
	return proc_id;
}

uintptr_t GetModuleBaseAddress(DWORD proc_id, const wchar_t* module_name)
{
	uintptr_t module_base_address = 0;
	HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32, proc_id);
	if (!h_snap)
	{
		MODULEENTRY32 module_entry = { 0 };
		module_entry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(h_snap, &module_entry))
		{
			do
			{
				if (!_wcsicmp(module_name, module_entry.szModule))
				{
					proc_id = module_entry.th32ProcessID;
					break;
				}
			} while (Module32Next(h_snap, &module_entry));
		}

		CloseHandle(h_snap);
	}
	return proc_id;
}

uintptr_t FindDMAAddy(HANDLE h_proc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(h_proc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}