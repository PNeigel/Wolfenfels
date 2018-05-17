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
	void UpdateKeystates();

	string ReadShaderGLSL(string filename);
	void CreateShaders();

	Player player;
	Stage stage;
	Enemy enemy;
	CollisionHandler coll;


	GLFWwindow* window;
	int keystates[8];

	vector<GLuint> shaders;
};

