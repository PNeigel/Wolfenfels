#pragma once

#include <GL/glew.h>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Door.h"
#include "Utils.h"
#include "Texture\TextureAtlas.h"

class Renderer
{
public:
	Renderer(Stage & stage, Enemy & enemy);
	Renderer(vector<GLuint>* shader);
	~Renderer();

	void RenderStageWalls(Stage & stage, GLuint shader);
	void RenderBG(Stage & stage, GLuint shader);
	void RenderAll(Stage & stage, GLuint* shader);

	void RenderPlayer(Player & player, GLuint shader);
	void RenderEnemy(Player & player, Enemy & enemy, GLuint shader);
	void renderDoor(Player & player, Door & door, GLuint shader);

	void RenderLine(Player & player, GLuint shader, glm::vec3 start, glm::vec3 end);

	vector<BasicTexture*> m_textures;

	vector<GLuint>* shader;
};

