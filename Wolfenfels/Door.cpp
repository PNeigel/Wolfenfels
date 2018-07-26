#include "Door.h"
#include "ResourceManager.h"
#include <ext.hpp>

Model* Door::m_model = nullptr;

Door::Door(glm::vec3 pos, bool xAligned) :
	m_pos(pos), m_xAligned(xAligned)
{
	if (m_model == nullptr)
		m_model = ResourceManager::addDoorModel();
	m_modelMatrix = glm::mat4(1.0f);
	if (m_xAligned)
		m_modelMatrix = glm::rotate(glm::radians(90.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
	m_modelMatrix = glm::translate(pos) * m_modelMatrix;
	m_openAnimationMesh.addKeyframe(glm::mat4(1.0), 0.0f);
	glm::vec3 transVec{1.0f, 0.0f, 0.0f};
	m_openAnimationMesh.addKeyframe(glm::translate(0.78*transVec), 1.0f);
	m_openAnimationMesh.reverse();

	if (!m_xAligned) {
		m_collisionRects[0] = Rect(-0.5f, -0.5f, 0.1f, 1.0f);
		m_collisionRects[1] = Rect(0.4f, -0.5f, 0.1f, 1.0f);
		m_collisionRects[2] = Rect(-0.4f, -0.1f, 0.8f, 0.2f);
	}
	else {
		m_collisionRects[0] = Rect(-0.5f, -0.5f, 1.0f, 0.1f);
		m_collisionRects[1] = Rect(-0.5f, 0.4f, 1.0f, 0.1f);
		m_collisionRects[2] = Rect(-0.1f, -0.4f, 0.2f, 0.8f);
	}

	m_openAnimationCollision.addKeyframe(m_collisionRects[2], 0.0f);

	if (!m_xAligned) {
		m_openAnimationCollision.addKeyframe(m_collisionRects[2].translate(glm::vec2(0.78f, 0.0f)), 1.0f);
	}
	else m_openAnimationCollision.addKeyframe(m_collisionRects[2].translate(glm::vec2(0.0f, 0.78f)), 1.0f);

	m_openAnimationCollision.reverse();
}


Door::~Door()
{
}

void Door::tick(float delta_time, CollisionHandler& coll, Player& player)
{
	m_openAnimationMesh.tick(delta_time);
	m_openAnimationCollision.tick(delta_time);
	glm::mat4 newAnimMatrix = m_openAnimationMesh.getCurrentKeyframe();
	m_animMatrix = newAnimMatrix;
	m_collisionRects[2] = m_openAnimationCollision.getCurrentKeyframe();

	if (coll.CheckOverlap(player.collision_rect, m_collisionRects[2].translate(m_pos))) {
		pushBackPlayer(player);
	}
}

void Door::toggle()
{
	m_openAnimationMesh.m_playing = true;
	m_openAnimationCollision.m_playing = true;
	m_openAnimationMesh.reverse();
	m_openAnimationCollision.reverse();
}

void Door::pushBackPlayer(Player& player)
{
	float pushValue = 0.01;
	if (!m_xAligned) {
		if (player.pos.y > m_pos.y)
			player.pos.y += pushValue;
		else
			player.pos.y -= pushValue;
	}
	else {
		if (player.pos.x > m_pos.x)
			player.pos.x += pushValue;
		else
			player.pos.x -= pushValue;
	}
}
