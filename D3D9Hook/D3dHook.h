#pragma once

#include <Windows.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3d9.h>
#include <d3dx9.h>

#include "Hook.h"
#include "Utils.h"
#include "Math.h"

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

namespace  d3dhook
{
	
	class D3dHook
	{
	public:
		D3dHook(): b_init_(false), p_d3d_device_(nullptr)
		{
			memset(d3d9_device_, 0, sizeof(d3d9_device_));
		}
		bool b_init_;
		LPDIRECT3DDEVICE9 p_d3d_device_;
		tEndScene o_end_scene_;
		void* d3d9_device_[119];

		Hook end_scene_hook;
		bool HookEndScene(BYTE* hkEndScene);
		bool GetD3D9Device();
		void DisableHook();
		
	};

	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawLine(vec2 src, vec2 dst, float thickness, D3DCOLOR color, IDirect3DDevice9* dev);
}

