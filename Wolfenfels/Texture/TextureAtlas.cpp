#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(string filename, int texPerRow, int texPerCol) :
	texPerRow(texPerRow), texPerCol(texPerCol)
{
	loadFromPNG(filename);
}

array<GLfloat, 8> TextureAtlas::getUVcoords(glm::vec2 & index)
{
	const GLfloat singleTexWidth = 1.0f / (GLfloat)texPerRow;
	const GLfloat singleTexHeight = 1.0f / (GLfloat)texPerCol;
	const GLfloat PIXEL_WIDTH = 1.0f / (GLfloat)m_width;
	const GLfloat PIXEL_HEIGHT = 1.0f / (GLfloat)m_height;

	GLfloat xMin = (index.x * singleTexWidth) + 0.5f * PIXEL_WIDTH;
	GLfloat xMax = (xMin + singleTexWidth) - PIXEL_WIDTH;

	GLfloat yMax = (texPerCol - index.y) * singleTexHeight - 0.5f * PIXEL_HEIGHT;
	GLfloat yMin = (yMax - singleTexHeight) + PIXEL_HEIGHT;
	// (half) a pixel needs to be added/subtracted to make sure the coordinates lie in middle of pixel (no edge bleeding)


	return { xMin, yMin,
			xMax, yMin,
			xMax, yMax,
			xMin, yMax };
}
