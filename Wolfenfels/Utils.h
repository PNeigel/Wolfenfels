#pragma once

#include <array>
#include <vector>
#include <GL/glew.h>
#include "glm.hpp"

using namespace std;

struct Rect {

	Rect() {
		Rect(0.0, 0.0, 0.0, 0.0);
	}
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

	Rect translate(glm::vec2 shift) {
		return Rect(base.x + shift.x, base.y + shift.y, width, height);
	}

	Rect operator*(float scalar) {
		return Rect(base.x * scalar, base.y * scalar, width * scalar, height * scalar);
	}

	Rect operator+(Rect& rect) {
		return Rect(base.x + rect.base.x, base.y + rect.base.y, width + rect.width, height + rect.height);
	}

	glm::vec2 base;
	float height, width;
};

inline vector<GLfloat> BufferFromIndex(GLfloat* dict, int dictSize, int dimensions, int* index, int indexSize)
{
	assert(dictSize % dimensions == 0);

	vector<GLfloat> result;

	for (int i = 0; i < indexSize; i++) {
		if (index[i] > dictSize / dimensions) throw;
		for (int j = 0; j < dimensions; j++) {
			result.push_back(dict[index[i]*dimensions+j]);
		}
	}

	return result;
}