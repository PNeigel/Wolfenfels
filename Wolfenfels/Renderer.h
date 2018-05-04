#pragma once

#include <GL/glew.h>
#include "Stage.h"
#include "Player.h"

class Renderer
{
public:
	Renderer(Stage & stage, Player & player);
	~Renderer();

	void PNGtoTexture(string filename, int width, int height, unsigned char* buffer);

	GLuint VAllocStageWalls(Stage &stage);
	GLuint VAllocWallTexture(unsigned char * texture_buffer);
	GLuint VAllocBG(Stage &stage);

	void RenderStageWalls(Stage & stage, GLuint shader, GLuint vao);
	void RenderBG(Stage & stage, GLuint shader, GLuint vao);
	void RenderAll(Stage & stage, Player & player, GLuint* shader);

	GLuint VAllocPlayersprite(Player & player);
	GLuint VAllocPlayerTexture(unsigned char * texture_buffer);

	void RenderPlayer(Player & player, GLuint shader, GLuint vao);
	
	GLuint wall_textureID;
	GLsizei wall_texture_width = 512;
	GLsizei wall_texture_height = 512;

	GLuint player_textureID;
	GLsizei player_texture_width = 747;
	GLsizei player_texture_height = 706;

	GLuint bg_vao;
	GLuint stage_walls_vao;

	GLuint player_vao;
};

