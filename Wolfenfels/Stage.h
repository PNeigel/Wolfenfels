#pragma once
#include <vector>
#include <glm.hpp>
#include <ext.hpp>
#include <png.hpp>

#include <GL/glew.h>

#include <iostream>

using namespace std;

struct Wall {
	// Since all walls have same height, a wall is perfectly defined by its two "side posts"
	glm::vec2 sides[2];
	Wall() : sides{ glm::vec2{ -0.5f, 0.0f},
		glm::vec2{ 0.5f, 0.0f} } {}
	Wall(glm::vec2 left, glm::vec2 right) {
		sides[0] = left;
		sides[1] = right;
	}
};

class Stage
{
public:
	Stage(int stage_no);
	Stage() {}
	~Stage();

	void SetWallVerts();
	void SetBGVerts();

	void ReadStageFromFile(string filename);

	vector<Wall> walls;
	float* vertcoords;
	float* wall_UV_coords;
	float bgverts[24];
	float bgcolors[24];
	int n_wallverts;
	int n_vertcoords;
};

