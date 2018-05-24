#include "CollisionHandler.h"

#include "Player.h"
#include "Stage.h"
#include <iostream>


CollisionHandler::CollisionHandler()
{
}


CollisionHandler::~CollisionHandler()
{
}

glm::vec2 CollisionHandler::TestMove(Player& player, Stage & stage, double delta_time, float direction)
{
	// test if the the players movement (vel * delta_time) will result in collision with any wall
	Rect future_x(player.collision_rect);
	Rect future_y(player.collision_rect);
	future_x.base.x += direction * player.view_dir.x * player.move_vel * delta_time;
	future_y.base.y += direction * player.view_dir.y * player.move_vel * delta_time;
	bool stop_x = false;
	bool stop_y = false;
	glm::vec2 move_vec(direction * player.view_dir);
	for (Wall wall : stage.walls) {
		if (!stop_x && CheckOverlap(future_x, wall.collision_rect)) {
			stop_x = true;
			move_vec.x = 0;
		}
		if (!stop_y && CheckOverlap(future_y, wall.collision_rect)) {
			stop_y = true;
			move_vec.y = 0;
		}
		if (stop_x && stop_y) break;
	}
	return move_vec;
}

glm::vec2 CollisionHandler::Slide(glm::vec2 move, Rect& rectA, Rect& rectB)
{
	float Ax1 = rectA.base.x;
	float Ax2 = Ax1 + rectA.width;
	float Ay1 = rectA.base.y;
	float Ay2 = Ay1 + rectA.height;

	float Bx1 = rectB.base.x;
	float Bx2 = Bx1 + rectB.width;
	float By1 = rectB.base.y;
	float By2 = By1 + rectB.height;

	float width_gap;

	return glm::vec2();
	
}

bool CollisionHandler::CheckOverlap(Rect& rectA, Rect& rectB)
{
	bool horizontal = false;
	bool vertical = false;
	if ((rectA.base.x + rectA.width > rectB.base.x) && (rectA.base.x < rectB.base.x + rectB.width)) {
		horizontal = true;
	}
	if (horizontal && rectA.base.y + rectA.height > rectB.base.y && rectA.base.y < rectB.base.y + rectB.height) {
		vertical = true;
	}
	if (vertical && horizontal) {
		return true;
	}
	return false;
}

bool CollisionHandler::rayLineIntersection(glm::vec2 start, glm::vec2 dir, float maxLength, glm::vec2 lineStart, glm::vec2 lineDir, float lineLength)
{
	float t; // t: length parameter for the casted ray
	float s; // s: length parameter along the tested line
	return rayLineIntersection(start, dir, maxLength, lineStart, lineDir, lineLength, t, s);
}

bool CollisionHandler::rayLineIntersection(glm::vec2 start, glm::vec2 dir, float maxLength, glm::vec2 lineStart, glm::vec2 lineDir, float lineLength, float& t, float&s)
{
	// t: length parameter for the casted ray
	// s: length parameter along the tested line

	float epsilon = 0.001;
	assert(glm::isNormalized(dir, epsilon));
	assert(glm::isNormalized(lineDir, epsilon));

	float tmp = dir.y / dir.x;
	s = (lineStart.y - start.y) - tmp * ( lineStart.x - start.x);
	s /= (tmp * lineDir.x - lineDir.y);

	t = lineStart.x - start.x + s * lineDir.x;
	t /= dir.x;

	bool result = true;
	if (t > maxLength || t < 0 || s < 0 || s > lineLength) {
		result = false;
		t = maxLength;
	}
	return result;
}
