#pragma once

#include <vector>
#include <array>
#include "glm.hpp"

using namespace std;

class TextureAnimation
{
public:

	struct rect {
		glm::vec2 base;
		float height, width;
		rect(float base_x, float base_y, float width, float height) :
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
	};
	TextureAnimation();
	~TextureAnimation();

	void Update();
	bool playing = false;
	bool repeat = false;
	array<float, 8> GetCurrentUV();

	unsigned int n_images;
	unsigned int n_frames;
	int current_frame = -1;
	vector<rect> UV_rects;
	vector<unsigned int> animation;
};

