#include "Door.h"
#include "ResourceManager.h"
#include <ext.hpp>

Door::Door(glm::vec3 pos) :
	m_pos(pos)
{
	m_model = ResourceManager::addDoorModel();
	m_modelMatrix = glm::translate(pos);
}


Door::~Door()
{
}
