#pragma once

#include <GL/glew.h>
#include "Stage.h"

class Renderer
{
public:
	Renderer(Stage & stage);
	~Renderer();

	void PNGtoTexture(string filename, int width, int height, unsigned char* buffer);

	GLuint VAllocStageWalls(Stage &stage);
	GLuint VAllocWallTexture(unsigned char * texture_buffer);
	GLuint VAllocBG(Stage &stage);

	void RenderStageWalls(Stage & stage, GLuint shader, GLuint vao);
	void RenderBG(Stage & stage, GLuint shader, GLuint vao);
	void RenderAll(Stage & stage, GLuint wallshader, GLuint bgshader);

	GLsizei wall_texture_width = 512;
	GLsizei wall_texture_height = 512;
	//unsigned char* wall_texture = new unsigned char[wall_texture_width * wall_texture_height * 4];

	GLuint wall_textureID;
	GLuint bg_vao;
	GLuint stage_walls_vao;
};

