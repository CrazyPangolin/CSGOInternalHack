// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "csgo_entitylist.h"

DWORD WINAPI HackThreadProc(_In_ HMODULE h_module);


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE h_thread = nullptr;
		h_thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThreadProc, hModule, 0, nullptr);
		if(h_thread)
		{
			CloseHandle(h_thread);
		}
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}



DWORD WINAPI HackThreadProc
(
	_In_ HMODULE h_module
)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"client.dll");

	CBaseEntityList* ent_list = (CBaseEntityList*)(moduleBase + 0x4DA2F44);


	//HMODULE h_module = *((HMODULE*)lpParameter);
	bool radar_show = false;
	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			radar_show = true;
		}

		for (auto e : ent_list->ent_list)
		{
			if (e.ent_ptr)
			{
				if (radar_show)
				{
					e.ent_ptr->is_spotted = true;
				}
				//std::cout << e.ent_ptr->health << "\n";
			}

		}

		//std::cout << "=========" << "\n";
		Sleep(1);
	}
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(h_module, 0);
}

