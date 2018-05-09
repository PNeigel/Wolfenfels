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
		// this loop intends to find the current image depending on the current animation time
		while (pos_help < current_time) {
			pos_help += animation[current_frame].duration;
			current_frame++;
		}
		int current_image = animation[current_frame - 1].frame;
		return UV_rects[current_image].AsArray();
	}
	else {// return zero array
		array<float, 8> zeros{ 0 };
		return zeros;
	}
}
