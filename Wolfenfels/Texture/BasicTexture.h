#pragma once

#include <GL/glew.h>
#include <string>

using namespace std;

class BasicTexture
{
	public:
		BasicTexture();
		BasicTexture(string filename);
		~BasicTexture();

		void loadFromPNG(string filename);

		void bind() const;

		GLuint m_id;

		int m_width;
		int m_height;
};

