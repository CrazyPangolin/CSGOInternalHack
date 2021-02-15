#pragma once

#include <Windows.h>

#include "csgo.hpp"
#include "Entity.h"
#include "Utils.h"
#include "D3dHook.h"



class Esp
{
private:
	float view_matrix_[16];
	int window_width_;
	int window_height_;
	HWND game_window_;
	uintptr_t module_base_;
	EntityInfo* entity_list_;
	Entity* local_player_;
	
public:
	Esp(uintptr_t module_base);
	
	bool WorldToScreen(vec3_f pos, vec2& screen);
	bool Update();
	bool DrawSnapLine(LPDIRECT3DDEVICE9 p_device);
};

