#pragma once

#include "glm.hpp"
#include "Model.h"
#include "Animation.h"

class Door
{
public:
	Door(glm::vec3 pos, bool xAligned);
	~Door();

	void tick(float delta_time);

	static Model* m_model;

	glm::vec3 m_pos;
	bool m_xAligned = false;
	
	glm::mat4 m_modelMatrix;
	glm::mat4 m_animMatrix = glm::mat4(1.0f);

	Animation<glm::mat4> m_openAnimation = Animation<glm::mat4>(true, false);
};

