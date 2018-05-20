#include "BasicTexture.h"

#include <png.hpp>

BasicTexture::BasicTexture()
{
}

BasicTexture::BasicTexture(string filename)
{
	loadFromPNG(filename);
}


BasicTexture::~BasicTexture()
{
	glDeleteTextures(1, &m_id);
}

void BasicTexture::loadFromPNG(string filename)
{
	png::image< png::rgba_pixel > image(filename);
	png::pixel_buffer<png::rgba_pixel> img_buf = image.get_pixbuf();
	
	m_width = image.get_width();
	m_height = image.get_height();
	unsigned char* buffer = new unsigned char[m_width * m_height * 4];
	
	for (int i = 0; i < m_height; ++i) {
		for (int j = 0; j < m_width; ++j) {
			buffer[(m_height - 1 - i) * m_width * 4 + j * 4 + 0] = img_buf.get_pixel(j, i).red;
			buffer[(m_height - 1 - i) * m_width * 4 + j * 4 + 1] = img_buf.get_pixel(j, i).green;
			buffer[(m_height - 1 - i) * m_width * 4 + j * 4 + 2] = img_buf.get_pixel(j, i).blue;
			buffer[(m_height - 1 - i) * m_width * 4 + j * 4 + 3] = img_buf.get_pixel(j, i).alpha;
		}
	}

	glGenTextures(1, &m_id);

	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	delete[] buffer;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void BasicTexture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}
