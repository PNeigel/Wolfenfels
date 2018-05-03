#include "Stage.h"

#include <fstream>
#include <sstream>


Stage::Stage(int stage_no)
{
	stringstream ssfilename;
	ssfilename << "Stages/stage" << std::to_string(stage_no) << ".txt";
	string filename = ssfilename.str();
	ReadStageFromFile(filename);
	SetWallVerts();
	SetBGVerts();
}


Stage::~Stage()
{
}

void Stage::SetWallVerts()
{	// Create the vertex positions for the VBO's
	vector<float>* vertcoords_vec = new vector<float>;
	for (uint16_t i = 0; i < walls.size(); i++) { // Set vertices for every wall. Top is z = 1.0, bottom z = 0.0
		vertcoords_vec->push_back(walls[i].sides[0].x);
		vertcoords_vec->push_back(walls[i].sides[0].y);
		vertcoords_vec->push_back(0.0f);
		vertcoords_vec->push_back(walls[i].sides[1].x);
		vertcoords_vec->push_back(walls[i].sides[1].y);
		vertcoords_vec->push_back(0.0f);
		vertcoords_vec->push_back(walls[i].sides[1].x);
		vertcoords_vec->push_back(walls[i].sides[1].y);
		vertcoords_vec->push_back(1.0f);
		vertcoords_vec->push_back(walls[i].sides[0].x);
		vertcoords_vec->push_back(walls[i].sides[0].y);
		vertcoords_vec->push_back(1.0f);
	}
	vertcoords = &(*vertcoords_vec)[0];
	n_vertcoords = vertcoords_vec->size();
	n_wallverts = walls.size() * 4;

	float UV_dict[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	vector<float>* UV_vec = new vector<float>;
	for (uint16_t i = 0; i < walls.size(); i++) { // Set colors for every wall.
		for (int j = 0; j < 8; j++) { // 12 is the number of components per quad (3*4)
			UV_vec->push_back(UV_dict[j]);
		}
	}
	wall_UV_coords = &(*UV_vec)[0];
}

void Stage::SetBGVerts()
{
	float lbgverts[] = {
		// ceiling
		-1.0f, 0.0f, 0.9999f,
		1.0f, 0.0f, 0.9999f,
		1.0f, 1.0f, 0.9999f,
		-1.0f, 1.0f, 0.9999f,
		//floor
		-1.0f, 0.0f, 0.9999f,
		-1.0f, -1.0f, 0.9999f,
		1.0f, -1.0f,0.9999f,
		1.0f, 0.0f, 0.9999f
	};
	float lbgcolors[] = {
		// ceiling
		0.53f, 0.81f,  0.98f,
		0.53f, 0.81f,  0.98f,
		0.53f, 0.81f,  0.98f,
		0.53f, 0.81f,  0.98f,
		// floor
		0.18f, 0.21f,  0.24f,
		0.18f, 0.21f,  0.24f,
		0.18f, 0.21f,  0.24f,
		0.18f, 0.21f,  0.24f
	};
	std::copy(std::begin(lbgverts), std::end(lbgverts), bgverts);
	std::copy(std::begin(lbgcolors), std::end(lbgcolors), bgcolors);
}

void Stage::ReadStageFromFile(string filename)
{
	ifstream file{ filename };
	if (!file) cout << "ERROR: Could not open " << filename << endl;
	
	Wall wall;
	float left_x, left_y, right_x, right_y;
	while (file) {
		file >> left_x >> left_y >> right_x >> right_y;
		wall = Wall(glm::vec2{ left_x, left_y }, glm::vec2{ right_x, right_y });
		walls.push_back(wall);
	}
	file.close();
}
