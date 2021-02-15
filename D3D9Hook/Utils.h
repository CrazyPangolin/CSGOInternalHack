#pragma once

#include <Windows.h>

namespace utils
{


	HWND GetProcessWindow();
	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
	
}

