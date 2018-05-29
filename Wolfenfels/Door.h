#pragma once

#include "glm.hpp"
#include "Model.h"

class Door
{
public:
	Door(glm::vec3 pos, bool xAligned);
	~Door();

	static Model* m_model;

	glm::vec3 m_pos;
	bool m_xAligned = false;
	
	glm::mat4 m_modelMatrix;
};

