#include "Enemy.h"
#include "Stage.h"
#include <iostream>


Enemy::Enemy(glm::vec3 pos) :
	pos(pos)
{
	model = ResourceManager::m_models[1];

	m_UV = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

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
}
