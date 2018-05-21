#pragma once
#include <vector>
#include <glm.hpp>
#include <ext.hpp>
#include <png.hpp>
#include "Utils.h"
#include "Enemy.h"

#include <GL/glew.h>

#include <iostream>

class Player;


using namespace std;

struct Wall {
	// Since all walls have same height, a wall is perfectly defined by its two "side posts"
	glm::vec2 sides[2];
	Wall() {
		Wall(glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
	}
	Wall(glm::vec2 left, glm::vec2 right) {
		sides[0] = left;
		sides[1] = right;
		if (left.x - right.x == 0) { // Wall along y-axis
			if (left.y > right.y) { // Wall facing in negative x
				collision_rect = Rect(left.x, right.y, coll_thickness, glm::abs(left.y - right.y));
			}
			else {
				collision_rect = Rect(left.x - coll_thickness, left.y, coll_thickness, glm::abs(left.y - right.y));
			}
			
		}
		else { // Wall along x-axis
			if (left.x < right.x) { // Wall facing negative y
				collision_rect = Rect(left.x, left.y, glm::abs(left.x - right.x), coll_thickness);
			}
			else {
				collision_rect = Rect(right.x, right.y - coll_thickness, glm::abs(left.x - right.x), coll_thickness);
			}
		}
	}
	float coll_thickness = 0.2;
	Rect collision_rect;
};

class Stage
{
public:
	Stage(int stage_no);
	Stage() {}
	~Stage();

	void SetWallVerts();
	void SetBGVerts();

	void ReadStageFromPNG(string filename);

	void Tick(double delta_time, Player & player);
	void RemoveEnemies();

	vector<Wall> walls;
	vector<Enemy> enemies;
	vector<GLfloat> vertcoords;
	vector<GLfloat> wall_UV_coords;
	array<GLfloat, 24> bgverts;
	array<GLfloat, 24> bgcolors;
};

