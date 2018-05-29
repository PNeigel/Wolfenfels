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
	Stage(int stage_no);
	Stage() {}
	~Stage();

	void SetWallVerts();
	void SetBGVerts();

	void ReadStageFromPNG(string filename);

	void Tick(double delta_time, CollisionHandler & coll, int* keystates);

	const float m_closeDistance = 5.0;

	Model* m_wallModel;
	Model* m_bgModel;

	Player player;
	vector<Wall> walls;
	vector<Enemy> enemies;
	vector<Door> m_doors;

	vector<GLfloat> wall_UV_coords;
	array<GLfloat, 24> bgcolors;
};

