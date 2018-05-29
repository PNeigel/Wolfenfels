#pragma once

#include "globalenums.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionHandler.h"
#include "Renderer.h"
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
	void UpdateKeystates(int key, int action);

	string ReadShaderGLSL(string filename);
	void CreateShaders();

	GLFWwindow* window;
	int keystates[9];

	vector<GLuint> shaders;
};

extern Engine engine;

