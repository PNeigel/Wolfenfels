#include "Player.h"
#include "globalenums.h"

#include <iostream>

using namespace std;


Player::Player()
{
	proj_mat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	ComputeView();
	SetSpriteCoords();
	weapon_anim.animation = { 0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4 };
	weapon_anim.UV_rects.push_back(TextureAnimation::rect(0.0f, 0.0f, 0.2, 1.0));
	weapon_anim.UV_rects.push_back(TextureAnimation::rect(0.2f, 0.0f, 0.2, 1.0));
	weapon_anim.UV_rects.push_back(TextureAnimation::rect(0.4f, 0.0f, 0.2, 1.0));
	weapon_anim.UV_rects.push_back(TextureAnimation::rect(0.605f, 0.0f, 0.2, 1.0));
	weapon_anim.UV_rects.push_back(TextureAnimation::rect(0.805f, 0.0f, 0.2, 1.0));
	weapon_anim.n_frames = 20;
	weapon_anim.n_images = 5;
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
	if (keystates[Buttons::SHOOT]) {
		weapon_anim.playing = true;
		keystates[Buttons::SHOOT] = false;
	}
	ComputeView();
	cout << "\r(" << pos.x << ", " << pos.y << ")";
	weapon_anim.Update();
	if (weapon_anim.playing) {
		array<float, 8> uv_array = weapon_anim.GetCurrentUV();
		std::copy(uv_array.begin(), uv_array.end(), sprite_UV_coords);
	}
	else {
		// If anim not playing set current UV to the standard UV
		// will be copied every frame that is not animated, performance?
		std::copy(begin(std_UV_coords), end(std_UV_coords), sprite_UV_coords);
	}
}

void Player::SetSpriteCoords()
{
	float lspritecoords[] = {
		-0.25f, -1.0f, 0.0f,
		0.15f, -1.0f, 0.0f,
		0.15f, 0.1f, 0.0f,
		-0.25f, 0.1f, 0.0f
	};
	std::copy(std::begin(lspritecoords), std::end(lspritecoords), spritecoords);

	float lstd_UV_coords[] = {
		0.0f, 0.0f,
		0.2f, 0.0f,
		0.2f, 1.0f,
		0.0f, 1.0f
	};
	std::copy(std::begin(lstd_UV_coords), std::end(lstd_UV_coords), std_UV_coords);
}
