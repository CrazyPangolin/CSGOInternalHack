// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include <iostream>
#include "Hook.h"
#include "opengl_draw.h"


typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers owglSwapBuffers;
twglSwapBuffers wglSwapBuffersGateway;



GL::Font gl_font;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

const char* example = "ESP Box";
const char* example2 = "I'm inside fam";

void Draw()
{
    HDC current_hdc = wglGetCurrentDC();

	if(!gl_font.bBuilt || current_hdc != gl_font.hdc)
	{
        gl_font.Build(FONT_HEIGHT);
	}
    GL::SetupOrtho();

    GL::DrawOutline(300, 300, 200, 200, 2.0f, rgb::red);

    float textPointX = gl_font.centerText(300, 200, strlen(example) * FONT_WIDTH);
    float textPointY = 300 - FONT_HEIGHT / 2;
    gl_font.Print(textPointX, textPointY, rgb::green, "%s", example);

    GL::vec3 insideTextPoint = gl_font.centerText(300, 300 + 100, 200, 200, strlen(example2) * FONT_WIDTH, FONT_HEIGHT);
    gl_font.Print(insideTextPoint.x, insideTextPoint.y, rgb::green, "%s", example2);

    GL::RestoreGL();
	
}

BOOL __stdcall hkwglSwapBuffers(HDC hDc)
{
    Draw();
    return wglSwapBuffersGateway(hDc);
}

DWORD WINAPI HackThread(HMODULE h_module)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"client_panorama.dll");


    Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);
    SwapBuffersHook.Enable();
	
    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            SwapBuffersHook.Disable();
            break;
        }
        Sleep(1000);
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(h_module, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
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

