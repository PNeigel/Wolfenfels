#pragma once
#include <vector>
#include <glm.hpp>
#include <ext.hpp>
#include <png.hpp>
#include "Utils.h"
#include "Enemy.h"
#include "ResourceManager.h"
#include "Model.h"
#include "Wall.h"
#include "Door.h"

#include <GL/glew.h>

#include <iostream>

class Player;


using namespace std;

class Stage
{
public:
	Stage(int stage_no, Player& player);
	Stage() {}
	~Stage();

	void SetWallVerts();
	void SetBGVerts();

	void ReadStageFromPNG(string filename);

	void Tick(double delta_time, Player & player);

	const float m_closeDistance = 5.0;

	Model* m_wallModel;
	Model* m_bgModel;

	vector<Wall> walls;
	vector<Enemy> enemies;
	Door m_door = Door(glm::vec3{0.5f, 2.5f, 0.0f});

	vector<GLfloat> wall_UV_coords;
	array<GLfloat, 24> bgcolors;
};

