#include "D3dHook.h"


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
	d3dpp.hDeviceWindow = utils::GetProcessWindow();

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

