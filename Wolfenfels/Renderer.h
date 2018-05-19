#pragma once

#include <GL/glew.h>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Utils.h"

class Renderer
{
public:
	Renderer(Stage & stage, Player & player, Enemy & enemy);
	Renderer(vector<GLuint>* shader);
	~Renderer();

	void PNGtoTexture(string filename, int width, int height, unsigned char* buffer);

	GLuint VAllocStageWalls(Stage &stage);
	GLuint VAllocWallTexture(unsigned char * texture_buffer);
	GLuint VAllocBG(Stage &stage);

	void RenderStageWalls(Stage & stage, Player & player, GLuint shader, GLuint vao);
	void RenderBG(Stage & stage, GLuint shader, GLuint vao);
	void RenderAll(Stage & stage, Player & player, GLuint* shader);

	GLuint VAllocPlayersprite(Player & player);
	void UpdatePlayerUV(float uv_xy[8]); // 8 = 4 * vec2 uv coords
	GLuint VAllocPlayerTexture(unsigned char * texture_buffer);

	GLuint VAllocEnemy(Enemy & enemy);
	GLuint VAllocEnemyTexture(unsigned char * texture_buffer);

	void RenderPlayer(Player & player, GLuint shader, GLuint vao);
	void RenderEnemy(Player & player, Enemy & enemy, GLuint shader, GLuint vao);

	void RenderLine(Player & player, GLuint shader, glm::vec3 start, glm::vec3 end);
	
	GLuint wall_textureID;
	GLsizei wall_texture_width = 512;
	GLsizei wall_texture_height = 512;

	GLuint player_textureID;
	GLsizei player_texture_width = 394;
	GLsizei player_texture_height = 103;

	GLuint bg_vao;
	GLuint stage_walls_vao;

	GLuint vbo_player_uv;
	GLuint player_vao;

	GLuint enemy_texID;
	GLsizei enemy_tex_width = 800;
	GLsizei enemy_tex_height = 817;

	GLuint enemy_vao;

	vector<GLuint>* shader;
};

