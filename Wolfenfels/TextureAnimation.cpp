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
			current_frame = 0;
			if (!repeat) playing = false;
		}
	}
	else current_frame = 0;
}

array<float, 8> TextureAnimation::GetCurrentUV()
{
	unsigned int current_image = animation[current_frame];
	return UV_rects[current_image].AsArray();
}
