#include "Esp.h"
#include <iostream>


Esp::Esp(uintptr_t module_base) : window_width_(0), window_height_(0), module_base_(module_base)
{
	memset(view_matrix_, 0, sizeof(view_matrix_));
	game_window_ = utils::GetProcessWindow();
	local_player_ = *(Entity**)(module_base_ + 0xD8B2BC);
}
Esp::~Esp()
{
	if (font_ != nullptr)
	{
		font_->Release();
	}
	if (line_ != nullptr)
	{
		line_->Release();
	}
}

bool Esp::Update()
{
	RECT window_rect;
	if (!GetWindowRect(game_window_, &window_rect))
	{
		return false;
	}

	window_width_ = window_rect.right - window_rect.left;
	window_height_ = window_rect.bottom - window_rect.top;
	if (window_width_ * window_height_ < 100)
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
	screen.x = screen.x < window_width_ ? screen.x : window_width_;
	screen.x = screen.x > 0 ? screen.x : 0;
	screen.y = screen.y < window_height_ ? screen.y : window_height_;
	screen.y = screen.y > 0 ? screen.y : 0;
	return true;
}

bool Esp::DrawSnapLine(LPDIRECT3DDEVICE9 p_device)
{
	entity_list_ = (EntityInfo*)(module_base_ + 0x4DA2F44);
	if (!entity_list_[0].ent)
	{
		return false;
	}
	vec3_f self_position = (*(local_player_)).pos;
	uintptr_t self_team_num = local_player_->team_number;
	int i = 0;
	vec2 self_screen_pos;
	WorldToScreen(self_position, self_screen_pos);
	while (entity_list_[i].ent)
	{
		if (CheakAvailable(*(entity_list_[i].ent)))
		{
			vec3_f target_position = entity_list_[i].ent->pos;
			vec2 target_screen_pos;
			WorldToScreen(target_position, target_screen_pos);
			if (entity_list_[i].ent->team_number == self_team_num)
			{
				Esp::DrawLine(self_screen_pos, target_screen_pos, 3, D3DCOLOR_ARGB(255, 255, 0, 0), p_device);
			}
			else
			{
				Esp::DrawLine(self_screen_pos, target_screen_pos, 3, D3DCOLOR_ARGB(255, 0, 255, 0), p_device);
			}
		}

		i++;
	}
	return true;
}


bool Esp::CheakAvailable(const Entity ent) const
{
	if (ent.health == 0 || ent.is_dormant == 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}


vec3_f Esp::GetBonePos(Entity* ent, int bone_idx)
{
	uintptr_t bone_ptr = ent->bone_matrix;
	vec3_f bone_pos;
	bone_pos.x = *(float*)(bone_ptr + 0x30 * bone_idx + 0x0C);
	bone_pos.y = *(float*)(bone_ptr + 0x30 * bone_idx + 0x1C);
	bone_pos.z = *(float*)(bone_ptr + 0x30 * bone_idx + 0x2C);
	return bone_pos;
}


bool Esp::DrawBonesIndex(LPDIRECT3DDEVICE9 p_device)
{
	entity_list_ = (EntityInfo*)(module_base_ + 0x4DA2F44);

	if (!entity_list_[0].ent || (entity_list_[0].ent->health <= 0))
	{
		return false;
	}
	size_t ent_idx = 1;

	while (entity_list_[ent_idx].ent)
	{
		for (size_t bone_idx = 0; bone_idx < 94; bone_idx++)
		{
			if (CheakAvailable(*(entity_list_[ent_idx].ent)))
			{
				vec3_f bone_pos = GetBonePos(entity_list_[ent_idx].ent, bone_idx);
				vec2 bone_screen_pos;
				WorldToScreen(bone_pos, bone_screen_pos);
				char bone_index_str[25];
				sprintf_s(bone_index_str, "%d", bone_idx);

				DrawText(bone_index_str, bone_screen_pos, D3DCOLOR_ARGB(255, 0, 255, 0), p_device);
			}

			bone_idx++;
		}
		ent_idx++;
	}
	return true;
}


bool Esp::DrawBonePartly(size_t entity_idx, std::vector<size_t> bone_idx_vec, D3DCOLOR color, LPDIRECT3DDEVICE9 p_device)
{
	std::vector<vec2> line_pointers;
	for (size_t idx = 0; idx < bone_idx_vec.size(); idx++)
	{
		vec3_f bone_pos = GetBonePos(entity_list_[entity_idx].ent, bone_idx_vec[idx]);
		vec2 bone_screen_pos;
		WorldToScreen(bone_pos, bone_screen_pos);
		line_pointers.emplace_back(bone_screen_pos);
	}
	DrawLines(line_pointers, 3, color, p_device);
	return true;
}

bool Esp::DrawBonesSkeleton(LPDIRECT3DDEVICE9 p_device)
{
	entity_list_ = (EntityInfo*)(module_base_ + 0x4DA2F44);

	if (!entity_list_[0].ent || (entity_list_[0].ent->health <= 0))
	{
		return false;
	}

	uintptr_t self_team_num = local_player_->team_number;
	size_t ent_idx = 1;
	while (entity_list_[ent_idx].ent)
	{
		if (CheakAvailable(*(entity_list_[ent_idx].ent)))
		{
			D3DCOLOR color;
			if (entity_list_[ent_idx].ent->team_number == self_team_num)
			{
				color = D3DCOLOR_ARGB(255, 0, 255, 0);
			}
			else
			{
				color = D3DCOLOR_ARGB(255, 255, 0, 0);
			}
			std::vector<size_t> head_pointers = { 8,10 };
			DrawBonePartly(ent_idx, head_pointers, color, p_device);

			std::vector<size_t> body_pointers = { 10 ,4 ,0 };
			DrawBonePartly(ent_idx, body_pointers, color, p_device);

			std::vector<size_t> right_arm_pointers = { 10, 38, 12, 14 };
			DrawBonePartly(ent_idx, right_arm_pointers, color, p_device);

			std::vector<size_t> left_arm_pointers = { 10 ,68 ,42 ,44 };
			DrawBonePartly(ent_idx, left_arm_pointers, color, p_device);

			std::vector<size_t> left_leg_pointers = { 0 ,70 ,76 ,72 };
			DrawBonePartly(ent_idx, left_leg_pointers, color, p_device);

			std::vector<size_t> right_leg_pointers = { 0 ,82 ,78 ,80 };
			DrawBonePartly(ent_idx, right_leg_pointers, color, p_device);
		}
		ent_idx++;
	}
	return true;
}


void Esp::DrawBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color, IDirect3DDevice9* dev)
{
	int height = abs(top.y - bot.y);
	std::vector<vec2> rect_points;
	vec2 tl, tr;
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;
	rect_points.emplace_back(tl);
	rect_points.emplace_back(tr);
	vec2 bl, br;
	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;
	rect_points.emplace_back(br);
	rect_points.emplace_back(bl);
	rect_points.emplace_back(tl);
	DrawLines(rect_points, thickness, color, dev);
}

bool Esp::DrawEnt2dBox(LPDIRECT3DDEVICE9 p_device)
{
	entity_list_ = (EntityInfo*)(module_base_ + 0x4DA2F44);

	if (!entity_list_[0].ent || (entity_list_[0].ent->health <= 0))
	{
		return false;
	}
	size_t ent_idx = 1;
	const uintptr_t self_team_num = local_player_->team_number;

	while (entity_list_[ent_idx].ent)
	{
		if (CheakAvailable(*(entity_list_[ent_idx].ent)))
		{
			vec3_f head_pos = GetBonePos(entity_list_[ent_idx].ent, 8);
			vec3_f foot_pos = entity_list_[ent_idx].ent->pos;
			vec2 head_screen_pos;
			WorldToScreen(head_pos, head_screen_pos);
			vec2 foot_screen_pos;
			WorldToScreen(foot_pos, foot_screen_pos);

			D3DCOLOR color;
			if (entity_list_[ent_idx].ent->team_number == self_team_num)
			{
				color = D3DCOLOR_ARGB(255, 0, 255, 0);
			}
			else
			{
				color = D3DCOLOR_ARGB(255, 255, 0, 0);
			}

			DrawBox2D(head_screen_pos, foot_screen_pos, 2, color, p_device);

			int height = abs(foot_screen_pos.y - head_screen_pos.y);
			int dx = (head_screen_pos.x - foot_screen_pos.x);
			vec2 bot_health;
			vec2 top_health;
			float health_percent = entity_list_[ent_idx].ent->health / 100.f;
			int health_height = height * health_percent;

			bot_health.x = foot_screen_pos.x - (height / 4) - 2;
			top_health.x = head_screen_pos.x - (height / 4) - 2 - (dx * (1 - health_percent));

			bot_health.y = foot_screen_pos.y;
			top_health.y = foot_screen_pos.y - health_height;
			DrawLine(bot_health, top_health, 2, D3DCOLOR_ARGB(255, 0, 0, 255), p_device);
		}

		ent_idx++;
	}
	return true;
}



void  Esp::DrawLines(std::vector<vec2> pointers, int thickness, D3DCOLOR color, IDirect3DDevice9* dev) {
	if (pointers.size() < 2)
	{
		return;
	}
	for (size_t pointer_idx = 0; pointer_idx < pointers.size() - 1; pointer_idx++)
	{
		DrawLine(pointers[pointer_idx], pointers[pointer_idx + 1], thickness, color, dev);
	}
}

void  Esp::DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color, IDirect3DDevice9* dev)
{
	if (src.x > window_width_ || src.x < 0 ||
		src.y > window_height_ || src.y < 0 ||
		dst.x > window_width_ || dst.x < 0 ||
		dst.y > window_height_ || dst.y < 0)
	{
		return;
	}
	if (!line_)
		D3DXCreateLine(dev, &line_);

	D3DXVECTOR2 line_pointers[2];

	line_pointers[0] = D3DXVECTOR2(src.x, src.y);
	line_pointers[1] = D3DXVECTOR2(dst.x, dst.y);

	line_->SetWidth(thickness);
	line_->Draw(line_pointers, 2, color);
}

void Esp::DrawText(const char* text, vec2 pos, D3DCOLOR color, IDirect3DDevice9* dev)
{
	RECT rect;
	if (!font_)
	{
		D3DXCreateFont(dev, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &font_);
	}

	SetRect(&rect, pos.x + 1, pos.y + 1, pos.x + 1, pos.y + 1);
	font_->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, pos.x, pos.y, pos.x, pos.y);
	font_->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);

}

void Esp::Draw(LPDIRECT3DDEVICE9 p_device)
{
	UpdateSettings();
	if (settings.b_show_menu_)
	{
		D3DCOLOR menu_off_color = D3DCOLOR_ARGB(255, 0, 255, 0);
		D3DCOLOR menu_on_color = D3DCOLOR_ARGB(255, 255, 0, 0);
		int menu_offset_x = window_width_ / 2;
		int menu_offset_y = 50;
		DrawText("Show Menu (INS)", vec2(menu_offset_x, menu_offset_y), menu_on_color, p_device);
		if (settings.b_snap_line_)
		{
			DrawText("Snaplines (F2)", vec2(menu_offset_x, menu_offset_y + 1 * 12), menu_on_color, p_device);
		}
		else
		{
			DrawText("Snaplines (F2)", vec2(menu_offset_x, menu_offset_y + 1 * 12), menu_off_color, p_device);
		}

		if (settings.b_box2d_)
		{
			DrawText("2D Box (F3)", vec2(menu_offset_x, menu_offset_y + 2 * 12), menu_on_color, p_device);
		}
		else
		{
			DrawText("2D Box (F3)", vec2(menu_offset_x, menu_offset_y + 2 * 12), menu_off_color, p_device);
		}
		if (settings.b_skeleton_)
		{
			DrawText("Skeleton (F4)", vec2(menu_offset_x, menu_offset_y + 3 * 12), menu_on_color, p_device);
		}
		else
		{
			DrawText("Skeleton (F4)", vec2(menu_offset_x, menu_offset_y + 3 * 12), menu_off_color, p_device);
		}
	}

	if (settings.b_snap_line_)
	{
		DrawSnapLine(p_device);
	}

	if (settings.b_box2d_)
	{
		DrawEnt2dBox(p_device);
	}

	if (settings.b_skeleton_)
	{
		DrawBonesSkeleton(p_device);
	}

}

void Esp::UpdateSettings()
{
	if (GetAsyncKeyState(button.dw_show_menu_btn) & 1)
		settings.b_show_menu_ = !settings.b_show_menu_;
	if (GetAsyncKeyState(button.dw_snap_line_btn) & 1)
		settings.b_snap_line_ = !settings.b_snap_line_;
	if (GetAsyncKeyState(button.dw_box2d_btn) & 1)
		settings.b_box2d_ = !settings.b_box2d_;
	if (GetAsyncKeyState(button.dw_skeleton_btn) & 1)
		settings.b_skeleton_ = !settings.b_skeleton_;
}


//void Esp::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
//{
//	D3DRECT BarRect = { x, y, x + w, y + h };
//	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
//}
