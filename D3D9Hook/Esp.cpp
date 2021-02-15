#include "Esp.h"



Esp::Esp(uintptr_t module_base): window_width_(0), window_height_(0), module_base_(module_base)
{
	memset(view_matrix_, 0, sizeof(view_matrix_));
	game_window_ = utils::GetProcessWindow();
	local_player_ = *(Entity**)(module_base_ + 0xD8B2BC);
}


bool Esp::Update()
{
	RECT window_rect;
	if(!GetClientRect(game_window_, &window_rect))
	{
		return false;
	}
	
	window_width_ = window_rect.right - window_rect.left;
	window_height_ = window_rect.bottom - window_rect.top;
	if(window_width_ * window_height_ < 100)
	{
		return false;
	}
	memcpy_s(view_matrix_, sizeof(view_matrix_), (float*)(module_base_ + hazedumper::signatures::dwViewMatrix), sizeof(view_matrix_));
	return true;
}

bool Esp::WorldToScreen(vec3_f pos, vec2& screen)
{
	//view_matrix-vector Product, multiplying world(eye) coordinates by projection view_matrix = clipCoords
	vec4_f clipCoords;
	clipCoords.x = pos.x * view_matrix_[0] + pos.y * view_matrix_[1] + pos.z * view_matrix_[2] + view_matrix_[3];
	clipCoords.y = pos.x * view_matrix_[4] + pos.y * view_matrix_[5] + pos.z * view_matrix_[6] + view_matrix_[7];
	clipCoords.z = pos.x * view_matrix_[8] + pos.y * view_matrix_[9] + pos.z * view_matrix_[10] + view_matrix_[11];
	clipCoords.w = pos.x * view_matrix_[12] + pos.y * view_matrix_[13] + pos.z * view_matrix_[14] + view_matrix_[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3_f NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (window_width_ / 2 * NDC.x) + (NDC.x + window_width_ / 2);
	screen.y = -(window_height_ / 2 * NDC.y) + (NDC.y + window_height_ / 2);
	return true;
}

bool Esp::DrawSnapLine(LPDIRECT3DDEVICE9 p_device)
{
	entity_list_ = (EntityInfo*)(module_base_ + 0x4DA2F44);
	if(!entity_list_[0].ent)
	{
		return false;
	}
	vec3_f self_position = (*(local_player_)).pos;
	uintptr_t self_team_num = local_player_->team_number;
	int i = 0;
	vec2 self_screen_pos;
	WorldToScreen(self_position, self_screen_pos);
	while(entity_list_[i].ent)
	{
		vec3_f target_position = entity_list_[i].ent->pos;
		vec2 target_screen_pos;
		WorldToScreen(target_position, target_screen_pos);
		if(entity_list_[i].ent->health>0)
		{
			if (entity_list_[i].ent->team_number == self_team_num)
			{
				d3dhook::DrawLine(self_screen_pos, target_screen_pos, 3, D3DCOLOR_ARGB(255, 255, 0, 0), p_device);
			}
			else
			{
				d3dhook::DrawLine(self_screen_pos, target_screen_pos, 3, D3DCOLOR_ARGB(255, 0, 255, 0), p_device);
			}
		}

		i++;
	}
	return true;
}
