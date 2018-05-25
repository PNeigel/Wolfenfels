#pragma once

#include "Player.h"
#include "glm.hpp"
#include "Utils.h"

struct Wall {
	// Since all walls have same height, a wall is perfectly defined by its two "side posts"
	Wall();
	Wall(glm::vec2 left, glm::vec2 right);
	~Wall();

	glm::vec2 sides[2];
	glm::vec2 m_normal;
	float coll_thickness = 0.2;
	Rect collision_rect;
};

