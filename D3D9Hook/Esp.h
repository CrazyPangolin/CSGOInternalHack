#pragma once

#include "Entity.h"

class Esp
{
private:
	float view_matrix[16];

public:
	
	bool WorldToScreen(vec3_f pos, vec2& screen);
	
};

