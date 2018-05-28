#pragma once

#include "glm.hpp"
#include "Model.h"

class Door
{
public:
	Door(glm::vec3 pos);
	~Door();

	Model* m_model;

	glm::vec3 m_pos;
	
	glm::mat4 m_modelMatrix;
};

