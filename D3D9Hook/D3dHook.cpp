#include "D3dHook.h"

HWND window = NULL;

BOOL CALLBACK d3dhook::EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}


HWND d3dhook::GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}


bool d3dhook::D3dHook::GetD3D9Device()
{
	if (!d3d9_device_)
	{
		return false;
	}

	IDirect3D9* p_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!p_d3d)
	{
		return false;
	}
	IDirect3DDevice9* p_dummy_device = NULL;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDeviceCreated = p_d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dpp.hDeviceWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &p_dummy_device);

	if (dummyDeviceCreated != S_OK)
	{
		// may fail in windowed fullscreen mode, trying again with windowed mode
		d3dpp.Windowed = !d3dpp.Windowed;

		dummyDeviceCreated = p_d3d->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.hDeviceWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &p_dummy_device);

		if (dummyDeviceCreated != S_OK)
		{
			p_d3d->Release();
			return false;
		}
	}
	memcpy(d3d9_device_, *reinterpret_cast<void***>(p_dummy_device), sizeof(d3d9_device_));

	p_dummy_device->Release();
	p_d3d->Release();
	return true;
}



bool d3dhook::D3dHook::HookEndScene(BYTE* hkEndScene)
{

	if (GetD3D9Device())
	{
		end_scene_hook.Init((BYTE*)d3d9_device_[42], hkEndScene, (BYTE*)&o_end_scene_, 7);
		end_scene_hook.Enable();
		return true;
	}
	else
	{
		return false;
	}

}

void d3dhook::D3dHook::DisableHook()
{
	end_scene_hook.Disable();
}

void d3dhook::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}