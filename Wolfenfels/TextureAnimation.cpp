#include "TextureAnimation.h"


TextureAnimation::TextureAnimation()
{
}


TextureAnimation::~TextureAnimation()
{
}

void TextureAnimation::Update(double delta_time)
{
	if (playing) {
		current_time += delta_time;
		if (current_time >= duration) {
			current_time = 0;
			if (!repeat) playing = false;
		}
	}
	else current_time = 0;
}

array<float, 8> TextureAnimation::GetCurrentUV()
{
	if (current_time >= 0) {
		double pos_help = 0;
		int current_frame = 0;
		while (pos_help < current_time) {
			pos_help += m_animation[current_frame].duration;
			current_frame++;
		}
		glm::vec2 index = m_animation[current_frame - 1].texIndex;

		return m_texAtlas->getUVcoords(index);

		//return UV_rects[current_image].AsArray();
	}
	else {// return zero array
		array<float, 8> zeros{ 0 };
		return zeros;
	}
}
