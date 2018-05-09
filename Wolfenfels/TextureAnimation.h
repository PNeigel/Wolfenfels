#pragma once

#include <vector>
#include <array>
#include "glm.hpp"
#include "Utils.h"

using namespace std;

class TextureAnimation
{
public:

	struct FrameDuration {
		unsigned int frame;
		double duration;
	};

	TextureAnimation();
	~TextureAnimation();

	void Update(double delta_time);
	bool playing = false;
	bool repeat = false;
	array<float, 8> GetCurrentUV();

	double current_time = 0;
	double duration;
	vector<Rect> UV_rects;
	vector<FrameDuration> animation;
};

