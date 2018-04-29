#pragma once
#include <vector>
#include <glm.hpp>
#include <ext.hpp>
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
	Stage() { Stage(1); };
	~Stage();

	void RenderStage();
	void SetWallVerts();
	void CreateBufferArray();

	vector<Wall> walls;
	float* vertcoords;
	float* colors;
	int n_wallverts;
	int n_vertcoords;

	GLuint vbo_pos = 0; // Position VBO
	GLuint vbo_col = 0; // Color VBO
	GLuint vao = 0; // Vertex array object

};

