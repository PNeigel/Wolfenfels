#pragma once
#include "BasicTexture.h"

#include <array>
#include "glm.hpp"

class TextureAtlas :
	public BasicTexture
{
public:
	TextureAtlas(string filename, int texPerRow, int texPerCol);

	array<GLfloat, 8> getUVcoords(glm::vec2& index);

	int texPerRow;
	int texPerCol;
};

