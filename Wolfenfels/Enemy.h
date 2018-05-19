#pragma once

#include "Player.h"
#include "glm.hpp"
#include "Utils.h"

class Stage;

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Tick(double delta_time, Player & player, Stage & stage);

	int max_hp;
	int current_hp;

	glm::vec3 pos;
	glm::vec2 dir;
	float yaw = 0;

	glm::mat4 model_matrix;
	float mesh[12];

	float UV[8];

	float coll_width = 0.6;
};

