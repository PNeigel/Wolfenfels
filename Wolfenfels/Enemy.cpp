#include "Enemy.h"
#include "Stage.h"
#include <iostream>

Model* Enemy::model = nullptr;

Enemy::Enemy(glm::vec3 pos) :
	pos(pos)
{
	if (model == nullptr)
		model = ResourceManager::addEnemyModel();

	m_UV = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	max_hp = 20;
	current_hp = max_hp;
	TextureAtlas* textat = (TextureAtlas*)model->m_texture;
	m_UV = textat->getUVcoords(glm::vec2{ 0, 0 });
	model->updateVBO(1, m_UV.size(), m_UV.data());
}


Enemy::~Enemy()
{
}

void Enemy::Tick(double delta_time, Player & player, Stage & stage)
{
	//pos = pos + glm::vec3{dir.x, dir.y, 0} *1.0 * delta_time;
	dir = player.pos - pos;
	dir = glm::normalize(dir);
	yaw = glm::angle(dir, glm::vec2{ 0.0f, -1.0f });
	if (dir.x <= 0) {
		yaw = glm::radians(360.0f) - yaw;
	}
	model_matrix = glm::rotate(yaw, glm::vec3{0.0f, 0.0f, 1.0f});
	model_matrix = glm::translate(pos) * model_matrix;

	TextureAtlas* textat = (TextureAtlas*)model->m_texture;
	
	if ((float)current_hp / (float)max_hp < 0.6) {
		m_UV = textat->getUVcoords(glm::vec2{1, 0});
	}

}
