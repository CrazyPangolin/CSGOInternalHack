#pragma once

#include <Windows.h>
#include <vector>

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
	
	ID3DXFont* font_;
	ID3DXLine* line_;


	////Menu settings
	//bool b_menu_;
	//bool b_2Dbox_;
	//bool b_snap_line_;
	//bool b_skeleton_;
	
public:
	Esp(uintptr_t module_base);
	~Esp();
	
	bool Update();
	bool DrawSnapLine(LPDIRECT3DDEVICE9 p_device);
	bool DrawBonesIndex(LPDIRECT3DDEVICE9 p_device); // For Debug
	bool DrawBonesSkeleton(LPDIRECT3DDEVICE9 p_device);
	bool DrawEnt2dBox(LPDIRECT3DDEVICE9 p_device);

private:

	bool WorldToScreen(vec3_f pos, vec2& screen);
	bool CheakAvailable(const Entity ent) const;
	vec3_f GetBonePos(Entity* ent, int bone_idx);
	bool DrawBonePartly(size_t entity_idx, std::vector<size_t> bone_idx_vec, D3DCOLOR color, LPDIRECT3DDEVICE9 p_device);
	
	void DrawText(const char* text, vec2 pos, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawLines(std::vector<vec2> src, int thickness, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color, IDirect3DDevice9* dev);
	
};

