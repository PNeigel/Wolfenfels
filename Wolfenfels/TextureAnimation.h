#pragma once

#include <vector>
#include <array>
#include "glm.hpp"
#include "Utils.h"

#include "Texture\TextureAtlas.h"

using namespace std;

class TextureAnimation
{
public:

	struct TexDuration {
		glm::vec2 texIndex;
		double duration;
	};

	TextureAnimation();
	~TextureAnimation();

	void Update(double delta_time);
	void calcDuration();
	bool playing = false;
	bool repeat = false;
	array<float, 8> GetCurrentUV();

	double current_time = 0;
	double duration;

	TextureAtlas* m_texAtlas;
	vector<TexDuration> m_animation;
};

