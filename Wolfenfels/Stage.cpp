#include "Stage.h"


Stage::Stage(int stage_no)
{
	switch (stage_no) {
	case 1:
	{
		Wall wall;
		walls.push_back(wall);
		Wall wall2(glm::vec2{0.5f, 0.0f}, glm::vec2{0.5f, 1.0f});
		walls.push_back(wall2);
		Wall wall3(glm::vec2{ 0.5f, 1.0f }, glm::vec2{ 1.5f, 1.0f });
		walls.push_back(wall3);
		break;
	}
	default:
		int x = 5;
	}
	SetWallVerts();
	CreateBufferArray();
}


Stage::~Stage()
{
}

void Stage::RenderStage()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, n_wallverts);
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

	// Vertex colors
	float colors_dict[] = {
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f,
		0.9f, 0.8f,  0.7f
	};
	vector<float>* colors_vec = new vector<float>;
	for (uint16_t i = 0; i < walls.size(); i++) { // Set colors for every wall.
		for (int j = 0; j < 12; j++) {
			colors_vec->push_back(colors_dict[j]);
		}
	}
	colors = &(*colors_vec)[0];

	cout << "n_walltverts: " << n_wallverts << endl;
	cout << "walls.size(): " << walls.size() << endl << endl;
	for (int i=0; i<n_vertcoords; i++) {
		if ((i + 1) % 3 == 0 && i != 0) cout << vertcoords[i] << endl;
		else cout << vertcoords[i] << ",\t";
	}
}

void Stage::CreateBufferArray()
{
	// Vertex buffer objects and Vertex array objects

	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, n_vertcoords * sizeof(float), vertcoords, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, n_vertcoords * sizeof(float), colors, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}
