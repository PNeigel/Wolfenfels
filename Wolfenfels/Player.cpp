#include "Player.h"
#include "Stage.h"
#include "Enemy.h"

#include <iostream>

using namespace std;


Player::Player()
{
	weapon_anim.m_animation = { TextureAnimation::TexDuration{ glm::vec2{ 0, 0 }, 0.03 },
		TextureAnimation::TexDuration{ glm::vec2{ 1, 0 }, 0.03 },
		TextureAnimation::TexDuration{ glm::vec2{ 2, 0 }, 0.06 },
		TextureAnimation::TexDuration{ glm::vec2{ 3, 0 }, 0.06 },
		TextureAnimation::TexDuration{ glm::vec2{ 4, 0 }, 0.06 } };
	weapon_anim.duration = 0.24;
	SetSpriteCoords();
	initVBOs();
	initVAO();
	proj_mat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
	collision_rect = Rect(pos.x-coll_width/2.0, pos.y-coll_height/2.0, coll_width, coll_height);
	ComputeView();
	
}


Player::~Player()
{
	glDeleteBuffers(1, &m_vertVBO);
	glDeleteBuffers(1, &m_uvVBO);
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

void Player::Move(double delta_time, CollisionHandler& coll, Stage& stage, int* keystates)
{
	// Forwards, backwards
	if (keystates[Buttons::UP]) {
		glm::vec2 move = coll.TestMove(*this, stage, delta_time, 1.0);
		pos.x += move.x * move_vel * delta_time;
		pos.y += move.y * move_vel * delta_time;
	}
	else if (keystates[Buttons::DOWN]) {
		glm::vec2 move = coll.TestMove(*this, stage, delta_time, -1.0);
		pos.x += move.x * move_vel * delta_time;
		pos.y += move.y * move_vel * delta_time;
	}

	if (keystates[Buttons::LEFT]) yaw += yaw_vel * delta_time;
	else if (keystates[Buttons::RIGHT]) yaw -= yaw_vel * delta_time;
	if (yaw > 360) yaw -= 360;
	if (yaw < 0) yaw += 360;
}

void Player::Update(double delta_time, CollisionHandler& coll, Stage& stage, int* keystates)
{
	if (shoot_cd > 0) shoot_cd -= delta_time;
	Move(delta_time, coll, stage, keystates);
	collision_rect = Rect(pos.x - coll_width / 2.0, pos.y - coll_height / 2.0, coll_width, coll_height);
	if (keystates[Buttons::SHOOT] && shoot_cd <= 0.0) {
		Shoot(stage.enemies);
		shoot_cd = 0.5;
	}
	ComputeView();
	cout << "\r(" << pos.x << ", " << pos.y << ")";
	weapon_anim.Update(delta_time);
	array<GLfloat, 8> new_UV = weapon_anim.GetCurrentUV();
	if (new_UV != sprite_UV_coords)
		updateUV(new_UV);
}

void Player::Shoot(vector<Enemy> & enemies)
{
	weapon_anim.playing = true;

	// check collision with enemies
	// check if view_direction of player is between vectors to left and right side of enemy collision rect

	for (Enemy &enemy : enemies) {
		glm::vec2 perp{ enemy.dir.y * -1.0f, enemy.dir.x };
		glm::vec2 right = glm::vec2(enemy.pos) + perp * enemy.coll_width/2.0 - glm::vec2(pos);
		right = glm::normalize(right);
		glm::vec2 left = glm::vec2(enemy.pos) - perp * enemy.coll_width/2.0 - glm::vec2(pos);
		left = glm::normalize(left);
		float alpha = glm::angle(right, left);
		float toLeft = glm::angle(left, glm::vec2(view_dir));
		float toRight = glm::angle(right, glm::vec2(view_dir));
		if (toLeft < alpha && toRight < alpha) {
			int damage = 10;
			enemy.current_hp -= damage;
			cout << "Hit! New HP: " << enemy.current_hp << endl;
		}
	}

}

void Player::SetSpriteCoords()
{
	spritecoords = {
		-0.27f, -1.0f, -1.0f,
		0.13f, -1.0f, -1.0f,
		0.13f, 0.1f, -1.0f,
		-0.27f, 0.1f, -1.0f
	};

	//sprite_UV_coords = weapon_anim.GetCurrentUV();
	sprite_UV_coords = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
}

void Player::updateUV(array<GLfloat, 8> uv_coords)
{
	sprite_UV_coords = uv_coords;
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), sprite_UV_coords.data(), GL_STATIC_DRAW);
}

void Player::initVBOs()
{
	glGenBuffers(1, &m_vertVBO); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertVBO); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), spritecoords.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_uvVBO); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), sprite_UV_coords.data(), GL_STATIC_DRAW);
}

void Player::initVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
