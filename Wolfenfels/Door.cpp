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
	m_openAnimation.addKeyframe(glm::mat4(1.0), 0.0f);
	glm::vec3 transVec{1.0f, 0.0f, 0.0f};
	m_openAnimation.addKeyframe(glm::translate(0.78*transVec), 1.0f);
}


Door::~Door()
{
}

void Door::tick(float delta_time)
{
	m_openAnimation.tick(delta_time);
	glm::mat4 newAnimMatrix = m_openAnimation.getCurrentKeyframe();
	m_animMatrix = newAnimMatrix;
}
