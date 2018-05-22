#pragma once

#include "Texture/BasicTexture.h"

#include <GL/glew.h>
#include <vector>
#include <array>

class Model
{
	public:
		Model();
		~Model();


		void addVBO(int location, int data_dim, int data_size, GLfloat* data);
		void updateVBO(int i_vbo, int data_size, GLfloat* data);
		void genVAO();

		void bindVAO();

		BasicTexture* m_texture;

		std::vector<GLuint> m_VBOs;
		GLuint m_VAO;
};