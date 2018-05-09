#include "TextureAnimation.h"


TextureAnimation::TextureAnimation()
{
}


TextureAnimation::~TextureAnimation()
{
}

void TextureAnimation::Update()
{
	if (playing) {
		current_frame++;
		if (current_frame >= n_frames) {
			current_frame = -1;
			if (!repeat) playing = false;
		}
	}
	else current_frame = -1;
}

array<float, 8> TextureAnimation::GetCurrentUV()
{
	if (current_frame >= 0) {
		unsigned int current_image = animation[current_frame];
		return UV_rects[current_image].AsArray();
	}
	else {// return zero array
		array<float, 8> zeros{ 0 };
		return zeros;
	}
}
