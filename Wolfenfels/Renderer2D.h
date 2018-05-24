#pragma once

#include <GL/glew.h>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "CollisionHandler.h"
#include "Utils.h"

class Renderer2D
{
public:
	Renderer2D(GLuint* shader);
	~Renderer2D();

	void RenderStageWalls(Stage & stage, Player& player);
	void RenderAll(Stage & stage, Player & player, CollisionHandler& coll);

	void RenderPlayer(Player & player);
	void RenderEnemy(Player & player, Enemy & enemy);

	void RenderRaycast(Stage& stage, Player& player, CollisionHandler& coll);

	GLuint* m_screenShader;

	float m_scale = 0.3;
};

