#pragma once

#include "Utils.h"
#include <glm.hpp>

class Player;
class Stage;

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	glm::vec2 TestMove(Player& player, Stage& stage, double delta_time, float direction);
	glm::vec2 Slide(glm::vec2 move, Rect& rectA, Rect& rectB);
	bool CheckOverlap(Rect& rectA, Rect& rectB);

	bool rayLineIntersection(glm::vec2 start, glm::vec2 dir, float maxLength, glm::vec2 lineStart, glm::vec2 lineDir, float lineLength);
	bool rayLineIntersection(glm::vec2 start, glm::vec2 dir, float maxLength, glm::vec2 lineStart, glm::vec2 lineDir, float lineLength, float& t, float&s);
};

