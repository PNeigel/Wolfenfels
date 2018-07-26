#pragma once

#include "glm.hpp"
#include "Model.h"
#include "Animation.h"
#include "Utils.h"
#include "Player.h"

class Door
{
public:
	Door(glm::vec3 pos, bool xAligned);
	~Door();

	void tick(float delta_time, CollisionHandler& coll, Player& player);
	void toggle();

	void pushBackPlayer(Player& player);

	static Model* m_model;

	glm::vec3 m_pos;
	// xAligned means the path through the door is parallel to the x axis.
	bool m_xAligned = false;
	
	glm::mat4 m_modelMatrix;
	glm::mat4 m_animMatrix = glm::mat4(1.0f);

	Rect m_collisionRects[3];

	Animation<glm::mat4> m_openAnimationMesh = Animation<glm::mat4>(true, false);
	Animation<Rect> m_openAnimationCollision = Animation<Rect>(true, false);
};

