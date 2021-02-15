﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include <iostream>

#include"D3dHook.h"
#include "Entity.h"

d3dhook::D3dHook d3d_hook;


HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 p_device)
{
    if (d3d_hook.b_init_ == false)
    {
        d3d_hook.p_d3d_device_ = p_device;
        d3d_hook.b_init_ = true;
    }


    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"client.dll");

    EntityInfo* entity_list = (EntityInfo*)(moduleBase + 0x4DA2F44);

	
    d3dhook::DrawFilledRect(200, 200, 200, 200, D3DCOLOR_ARGB(255, 255, 0, 0), p_device);
    //draw stuff here like so:

    return d3d_hook.o_end_scene_(p_device);
}

DWORD WINAPI HackThreadProc(HMODULE h_module)
{

 
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Hooked..." << std::endl;


    d3d_hook.HookEndScene((BYTE*)&hkEndScene);

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            d3d_hook.DisableHook();
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
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThreadProc, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

