#include "Math.h"


vec3_f Subtract(vec3_f src, vec3_f dst)
{
	vec3_f diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

vec3_f Divide(vec3_f src, float num)
{
	vec3_f vec;
	vec.x = src.x / num;
	vec.y = src.y / num;
	vec.z = src.z / num;

	return vec;
}

float Magnitude(vec3_f vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float DotProduct(vec3_f src, vec3_f dst)
{
	return src.x * dst.x + src.y * dst.y + src.z * dst.z;
}
