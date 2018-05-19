#include "Enemy.h"
#include "Stage.h"
#include <iostream>


Enemy::Enemy()
{
	pos = glm::vec3{ 4.0f, 4.0f, 0.0f };
	float lmesh[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.9f,
		-0.5f, 0.0f, 0.9f
	};
	std::copy(std::begin(lmesh), std::end(lmesh), mesh);

	float lUV[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	std::copy(std::begin(lUV), std::end(lUV), UV);

	max_hp = 100;
	current_hp = max_hp;
}


Enemy::~Enemy()
{
}

void Enemy::Tick(double delta_time, Player & player, Stage & stage)
{
	//pos = pos + glm::vec3{dir.x, dir.y, 0} *1.0 * delta_time;
	dir = player.pos - pos;
	dir = glm::normalize(dir);
	//yaw = glm::acos(glm::dot(dir, glm::vec2{0.0f, -1.0f}));
	yaw = glm::angle(dir, glm::vec2{ 0.0f, -1.0f });
	if (dir.x <= 0) {
		yaw = glm::radians(360.0f) - yaw;
	}
	cout << "YAW: " << yaw;
	model_matrix = glm::rotate(yaw, glm::vec3{0.0f, 0.0f, 1.0f});
	model_matrix = glm::translate(pos) * model_matrix;

	if (current_hp <= 0) {
		stage.RemoveEnemies();
	}
}
