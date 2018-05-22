#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::addVBO(int location, int data_dim, int data_size, GLfloat* data)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(GLfloat), data, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, data_dim, GL_FLOAT, GL_FALSE, 0, NULL);
	m_VBOs.push_back(vbo);
}

void Model::updateVBO(int i_vbo, int data_size, GLfloat * data)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[i_vbo]);
	glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(GLfloat), data, GL_STATIC_DRAW);
}

void Model::genVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
}

void Model::bindVAO()
{
	glBindVertexArray(m_VAO);
}
