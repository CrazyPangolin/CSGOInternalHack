#include "Esp.h"


bool Esp::WorldToScreen(vec3_f pos, vec2& screen)
{
	////view_matrix-vector Product, multiplying world(eye) coordinates by projection view_matrix = clipCoords
	//vec4_f clipCoords;
	//clipCoords.x = pos.x * view_matrix[0] + pos.y * view_matrix[1] + pos.z * view_matrix[2] + view_matrix[3];
	//clipCoords.y = pos.x * view_matrix[4] + pos.y * view_matrix[5] + pos.z * view_matrix[6] + view_matrix[7];
	//clipCoords.z = pos.x * view_matrix[8] + pos.y * view_matrix[9] + pos.z * view_matrix[10] + view_matrix[11];
	//clipCoords.w = pos.x * view_matrix[12] + pos.y * view_matrix[13] + pos.z * view_matrix[14] + view_matrix[15];

	//if (clipCoords.w < 0.1f)
	//	return false;

	////perspective division, dividing by clip.W = Normalized Device Coordinates
	//vec3_f NDC;
	//NDC.x = clipCoords.x / clipCoords.w;
	//NDC.y = clipCoords.y / clipCoords.w;
	//NDC.z = clipCoords.z / clipCoords.w;

	//screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	//screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}
