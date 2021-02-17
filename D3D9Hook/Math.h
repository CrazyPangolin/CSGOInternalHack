#pragma once

#include <math.h>

#define PI 3.1415927f



struct vec2
{
	vec2() : x(0), y(0) { }
	vec2(const int x_v, const int y_v) : x(x_v), y(y_v) { }
	int x;
	int y;
};

struct vec4_f
{
	float x;
	float y;
	float z;
	float w;
};


struct vec3_f
{
	float x;
	float y;
	float z;
};

vec3_f Subtract(vec3_f src, vec3_f dst);

vec3_f Divide(vec3_f src, float num);

float Magnitude(vec3_f vec);

float DotProduct(vec3_f src, vec3_f dst);