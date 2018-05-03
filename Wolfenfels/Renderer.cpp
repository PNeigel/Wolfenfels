#include "Renderer.h"

#include <png.hpp>


Renderer::Renderer(Stage & stage)
{
	unsigned char* wall_texture_buffer = new unsigned char[wall_texture_width * wall_texture_height * 4];
	PNGtoTexture("wall.png", wall_texture_height, wall_texture_width, wall_texture_buffer);
	wall_textureID = VAllocWallTexture(wall_texture_buffer);
	delete[] wall_texture_buffer;
	bg_vao = VAllocBG(stage);
	stage_walls_vao = VAllocStageWalls(stage);
}


Renderer::~Renderer()
{
}

void Renderer::PNGtoTexture(string filename, int width, int height, unsigned char * buffer)
{
	png::image< png::rgba_pixel > image(filename);
	png::pixel_buffer<png::rgba_pixel> img_buf = image.get_pixbuf();
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			buffer[(height - 1 - i) * width * 4 + j * 4 + 0] = img_buf.get_pixel(j, i).red;
			buffer[(height - 1 - i) * width * 4 + j * 4 + 1] = img_buf.get_pixel(j, i).green;
			buffer[(height - 1 - i) * width * 4 + j * 4 + 2] = img_buf.get_pixel(j, i).blue;
			buffer[(height - 1 - i) * width * 4 + j * 4 + 3] = img_buf.get_pixel(j, i).alpha;
		}
	}
}

GLuint Renderer::VAllocStageWalls(Stage & stage)
{
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, stage.n_vertcoords * sizeof(float), stage.vertcoords, GL_STATIC_DRAW);

	/*
	glGenBuffers(1, &vbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, n_vertcoords * sizeof(float), colors, GL_STATIC_DRAW);
	*/

	GLuint vbo_uv = 0;
	glGenBuffers(1, &vbo_uv); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, stage.n_wallverts * 2 * sizeof(float), stage.wall_UV_coords, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

GLuint Renderer::VAllocWallTexture(unsigned char * texture_buffer)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wall_texture_width, wall_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

GLuint Renderer::VAllocBG(Stage & stage)
{
	GLuint vbo_pos = 0;
	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), stage.bgverts, GL_STATIC_DRAW);

	GLuint vbo_col = 0;
	glGenBuffers(1, &vbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), stage.bgcolors, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	return vao;
}

void Renderer::RenderStageWalls(Stage & stage, GLuint shader, GLuint vao)
{
	glUseProgram(shader);
	glBindTexture(GL_TEXTURE_2D, wall_textureID);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, stage.n_wallverts);
}

void Renderer::RenderBG(Stage & stage, GLuint shader, GLuint vao)
{
	// Dont really need stage at the moment
	glUseProgram(shader);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 8);
}

void Renderer::RenderAll(Stage & stage, GLuint wallshader, GLuint bgshader)
{
	RenderBG(stage, bgshader, bg_vao);
	RenderStageWalls(stage, wallshader, stage_walls_vao);
}
