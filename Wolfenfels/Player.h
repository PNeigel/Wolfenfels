#pragma once

#include "globalenums.h"
#include "TextureAnimation.h"

#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>

class Player
{ // Acts both as the player and camera
public:

	const float FOV = 60.0f;
	const int WIDTH = 1366;
	const int HEIGHT = 768;

	Player();
	~Player();

	void ComputeView();
	void Move(int* keystates);

	void Update(int* keystates);

	void SetSpriteCoords();

	float spritecoords[12];
	float sprite_UV_coords[8];

	glm::vec3 pos{ 0.0f, -3.0f, 0.5f };
	float move_speed = 0.04f;

	float yaw = 00.0f;
	float yaw_delta = 1.5f;

	glm::mat4 view_mat{ 1.0f };
	glm::vec3 view_dir{ 1.0f };

	glm::mat4 proj_mat;

	glm::mat4 mvp;

	TextureAnimation weapon_anim;

};

