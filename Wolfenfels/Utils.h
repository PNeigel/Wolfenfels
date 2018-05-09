#pragma once
#include <array>
#include "glm.hpp"

using namespace std;

struct Rect {

	Rect(float base_x, float base_y, float width, float height) :
		width(width), height(height) {
		base = glm::vec2{ base_x, base_y };
	}
	
	array<float, 8> AsArray() {
		array<float, 8> result = {
			base.x, base.y,
			base.x + width, base.y,
			base.x + width, base.y + height,
			base.x, base.y + height
		};
		return result;
	}

	glm::vec2 base;
	float height, width;
};