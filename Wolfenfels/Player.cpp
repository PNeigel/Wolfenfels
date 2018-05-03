#include "Player.h"

#include <iostream>

using namespace std;


Player::Player()
{
	cout << glm::to_string(pos) << endl;
	proj_mat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	ComputeView();
	SetSpriteCoords();
}


Player::~Player()
{
}

void Player::ComputeView()
{
	view_mat = glm::rotate(glm::radians(90.0f), glm::vec3{ 1,0,0 });
	view_mat = glm::translate(pos) * view_mat;
	view_mat = view_mat * glm::rotate(glm::radians(yaw), glm::vec3{ 0,1,0 });
	view_dir = view_mat * glm::vec4{ 0,0,-1,0 };
	view_dir = glm::normalize(view_dir);
	view_mat = glm::inverse(view_mat);

	mvp = proj_mat * view_mat;
}

void Player::Move(int* keystates)
{
	// Forwards, backwards
	if (keystates[Buttons::UP]) {
		pos += view_dir * move_speed;
	}
	else if (keystates[Buttons::DOWN]) pos -= view_dir * move_speed;

	if (keystates[Buttons::LEFT]) yaw += yaw_delta;
	else if (keystates[Buttons::RIGHT]) yaw -= yaw_delta;
	if (yaw > 360) yaw -= 360;
	if (yaw < 0) yaw += 360;
}

void Player::Update(int* keystates)
{
	Move(keystates);
	ComputeView();
	cout << "\r";
	cout << "Yaw: " << yaw;
	cout << "\tPos: " << glm::to_string(pos);
}

void Player::SetSpriteCoords()
{
	float lspritecoords[] = {
		-0.2f, -1.0f, 0.0f,
		0.2f, -1.0f, 0.0f,
		0.2f, -0.2f, 0.0f,
		-0.2f, -0.2f, 0.0f
	};
	std::copy(std::begin(lspritecoords), std::end(lspritecoords), spritecoords);

	float lsprite_texture_coords[] = {
		-0.2f, -1.0f, 0.0f,
		0.2f, -1.0f, 0.0f,
		0.2f, -0.2f, 0.0f,
		-0.2f, -0.2f, 0.0f
	};
	std::copy(std::begin(lsprite_texture_coords), std::end(lsprite_texture_coords), sprite_texture_coords);
}
