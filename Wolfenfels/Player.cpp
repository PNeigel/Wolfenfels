#include "Player.h"
#include "Stage.h"
#include "Enemy.h"

#include <iostream>

using namespace std;


Player::Player()
{
	SetSpriteCoords();
	model = ResourceManager::addPlayerModel();
	weapon_anim.m_animation = { TextureAnimation::TexDuration{ glm::vec2{ 0, 0 }, 0.03 },
		TextureAnimation::TexDuration{ glm::vec2{ 1, 0 }, 0.03 },
		TextureAnimation::TexDuration{ glm::vec2{ 2, 0 }, 0.06 },
		TextureAnimation::TexDuration{ glm::vec2{ 3, 0 }, 0.06 },
		TextureAnimation::TexDuration{ glm::vec2{ 4, 0 }, 0.06 } };
	weapon_anim.duration = 0.24;
	weapon_anim.m_texAtlas = (TextureAtlas*)model->m_texture;
	proj_mat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);
	collision_rect = Rect(pos.x-coll_width/2.0, pos.y-coll_height/2.0, coll_width, coll_height);
	ComputeView();
	
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
		Shoot(stage, coll);
		shoot_cd = 0.5;
	}
	ComputeView();
	cout << "\r(" << pos.x << ", " << pos.y << ")";
	weapon_anim.Update(delta_time);
	array<GLfloat, 8> new_UV = weapon_anim.GetCurrentUV();
	if (new_UV != sprite_UV_coords)
		updateUV(new_UV);
}

void Player::Shoot(Stage & stage, CollisionHandler& coll)
{
	weapon_anim.playing = true;

	float maxShootRange = 3;
	float newMaxRange;

	Enemy* potentialHit = nullptr;

	// check collision with enemies
	for (Enemy &enemy : stage.enemies) {
		glm::vec2 perp{ -1.0f * enemy.dir.y, enemy.dir.x };
		glm::vec2 lineStart = glm::vec2(enemy.pos) - perp * enemy.coll_width / 2.0f;
		float t, s;
		bool hit = coll.rayLineIntersection(glm::vec2(pos), glm::vec2(view_dir), maxShootRange, lineStart, perp, enemy.coll_width, t, s);
		m_lastRaycast[0] = pos;
		m_lastRaycast[1] = pos + t * view_dir;
		if (hit) { // we flag the closest hit enemy as potential hit
			newMaxRange = t;
			potentialHit = &enemy;
			break;
		}
	}

	if (potentialHit == nullptr)
		return; // no enemy

	// check collision with walls
	// if we hit a wall closer than the potential enemy hit, its not a hit
	for (Wall & wall : stage.walls) {
		glm::vec2 lineDir = wall.sides[1] - wall.sides[0];
		glm::vec2 lineStart = wall.sides[0];
		float wallLength = 1.0;
		float t, s;
		bool hit = coll.rayLineIntersection(glm::vec2(pos), glm::vec2(view_dir), newMaxRange, lineStart, lineDir, wallLength, t, s);
		m_lastRaycast[0] = pos;
		m_lastRaycast[1] = pos + t * view_dir;
		if (hit) // hit a wall instead of enemy
			return;
	}
	int damage = 10;
	potentialHit->current_hp -= damage;
	cout << "Hit! New HP: " << potentialHit->current_hp << endl;
}

void Player::SetSpriteCoords()
{
	ResourceManager::m_playerMesh = {
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
	model->updateVBO(1, uv_coords.size(), uv_coords.data());
}
