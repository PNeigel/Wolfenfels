#pragma once

#include "globalenums.h"
#include "Texture/TextureAtlas.h"
#include "Animation.h"
#include "CollisionHandler.h"
#include "Model.h"

#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>

class Enemy;

class Player
{ // Acts both as the player and camera
	public:

		float FOV;// = 178.0f;
		int WIDTH;// = 1366;
		int HEIGHT;// = 768;

		Player();
		Player(glm::vec3 pos);
		~Player();

		void ComputeView();
		void Move(double delta_time, CollisionHandler& coll, Stage& stage, int* keystates);
		void Update(double delta_time, CollisionHandler& coll, Stage& stage, int* keystates);
		void Shoot(Stage & stage, CollisionHandler& coll);
		void SetSpriteCoords();
		void updateUV(array<GLfloat, 8> uv_coords);
		void readSettings(string filename);

		array<GLfloat, 12> spritecoords;
		array<GLfloat, 8> sprite_UV_coords;

		glm::vec3 pos{ 0.5f, 0.5f, 0.5f };
		float move_vel = 3.0f;

		float shoot_cd = 0.0;

		float yaw = 00.0f;
		float yaw_vel = 100.0f;

		Rect collision_rect;
		float coll_width = 0.2;
		float coll_height = 0.2;

		glm::mat4 view_mat{ 1.0f };
		glm::vec3 view_dir{ 1.0f };
		glm::mat4 proj_mat;
		glm::mat4 mvp;

		TextureAtlas* m_textureAtlas;
		Animation<glm::vec2> m_weaponAnim = Animation<glm::vec2>(false, false);

		Model* model;

		glm::vec3 m_lastRaycast[2];
};

