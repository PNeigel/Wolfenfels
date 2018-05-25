#include "Wall.h"

#include <iostream>


Wall::Wall()
{
	Wall(glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
}

Wall::Wall(glm::vec2 left, glm::vec2 right)
{
	sides[0] = left;
	sides[1] = right;

	glm::vec2 parallel = right - left;
	m_normal.x = -1.0f * parallel.y;
	m_normal.y = parallel.x;

	m_normal = glm::normalize(m_normal);

	if (left.x - right.x == 0) { // Wall along y-axis
		if (left.y > right.y) { // Wall facing in negative x
			collision_rect = Rect(left.x, right.y, coll_thickness, glm::abs(left.y - right.y));
		}
		else {
			collision_rect = Rect(left.x - coll_thickness, left.y, coll_thickness, glm::abs(left.y - right.y));
		}

	}
	else { // Wall along x-axis
		if (left.x < right.x) { // Wall facing negative y
			collision_rect = Rect(left.x, left.y, glm::abs(left.x - right.x), coll_thickness);
		}
		else {
			collision_rect = Rect(right.x, right.y - coll_thickness, glm::abs(left.x - right.x), coll_thickness);
		}
	}
}


Wall::~Wall()
{
}
