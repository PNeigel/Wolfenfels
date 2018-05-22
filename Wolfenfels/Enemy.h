#pragma once

#include "Player.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "Model.h"

#include "glm.hpp"
#include <array>

class Stage;

class Enemy
{
public:
	Enemy(glm::vec3 pos);
	~Enemy();

	void Tick(double delta_time, Player & player, Stage & stage);

	int max_hp;
	int current_hp;

	glm::vec3 pos;
	glm::vec2 dir;
	float yaw = 0;

	Model* model;

	std::array<GLfloat, 8> m_UV;

	float coll_width = 0.6;

	glm::mat4 model_matrix;
};

