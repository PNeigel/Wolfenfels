#pragma once

#include <GL/glew.h>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Utils.h"
#include "Texture\TextureAtlas.h"

class Renderer
{
public:
	Renderer(Stage & stage, Player & player, Enemy & enemy);
	Renderer(vector<GLuint>* shader);
	~Renderer();

	void RenderStageWalls(Stage & stage, Player & player, GLuint shader, GLuint vao);
	void RenderBG(Stage & stage, GLuint shader, GLuint vao);
	void RenderAll(Stage & stage, Player & player, GLuint* shader);

	GLuint VAllocEnemy(Enemy & enemy);

	void RenderPlayer(Player & player, GLuint shader, GLuint vao);
	void RenderEnemy(Player & player, Enemy & enemy, GLuint shader, GLuint vao);

	void RenderLine(Player & player, GLuint shader, glm::vec3 start, glm::vec3 end);

	vector<BasicTexture*> m_textures;

	GLuint enemy_vao;

	vector<GLuint>* shader;
};

