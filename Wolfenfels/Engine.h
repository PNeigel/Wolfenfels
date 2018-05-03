#pragma once

#include "globalenums.h"
#include "Player.h"
#include "Stage.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <iostream>

class Engine
{
public:

	const int WIDTH = 1366;
	const int HEIGHT = 768;

	Engine();
	~Engine();

	bool Init();
	void GameLoop();
	void UpdateKeystates();

	void CreateShaders();

	Player player;
	Stage stage;

	GLFWwindow* window;
	int keystates[8];

	GLuint shader_program;
	GLuint bgshader_program;
	GLuint texshader_program;
};

