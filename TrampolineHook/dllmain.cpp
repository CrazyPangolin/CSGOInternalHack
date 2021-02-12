// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>
#include "mem.h"

typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);

twglSwapBuffers owglSwapBuffers;

BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
    std::cout << "Hooked" << std::endl;
    return owglSwapBuffers(hDc);
}
DWORD WINAPI HackThread(HMODULE h_module)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"client_panorama.dll");


    owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
    owglSwapBuffers = (twglSwapBuffers)mem::TrampHook32((char*)owglSwapBuffers, (char*)hkwglSwapBuffers, 5);
    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
        Sleep(1000);
    }
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(h_module, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	    {
        HANDLE hThread = nullptr;
        hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
        if (hThread)
        {
            CloseHandle(hThread);
        }
	    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

