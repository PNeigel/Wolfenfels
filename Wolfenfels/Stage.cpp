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
	CreateBufferArray();
	ReadPNG("wall.png");
}


Stage::~Stage()
{
}

void Stage::RenderStage(GLuint bgshader, GLuint wallshader)
{
	glUseProgram(bgshader);
	// Render floor and ceiling
	glBindVertexArray(bgvao);
	glDrawArrays(GL_QUADS, 0, 8);

	glUseProgram(wallshader);
	// Render Walls
	glBindTexture(GL_TEXTURE_2D, textureID);
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
		0.1f, 0.1f,  0.1f,
		0.3f, 0.3f,  0.3f,
		0.1f, 0.1f,  0.1f,
		0.2f, 0.2f,  0.2f
	};
	float UV_dict[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	vector<float>* colors_vec = new vector<float>;
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

void Stage::CreateBufferArray()
{
	// Vertex buffer objects and Vertex array objects

	glGenBuffers(1, &vbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, n_vertcoords * sizeof(float), vertcoords, GL_STATIC_DRAW);

	/*
	glGenBuffers(1, &vbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, n_vertcoords * sizeof(float), colors, GL_STATIC_DRAW);
	*/

	glGenBuffers(1, &vbo_uv); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, n_wallverts * 2 * sizeof(float), wall_UV_coords, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// Background

	glGenBuffers(1, &bgvbo_pos); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, bgvbo_pos); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), bgverts, GL_STATIC_DRAW);

	glGenBuffers(1, &bgvbo_col); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, bgvbo_col); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), bgcolors, GL_STATIC_DRAW);

	glGenVertexArrays(1, &bgvao);
	glBindVertexArray(bgvao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bgvbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, bgvbo_col);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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

void Stage::ReadPNG(string filename)
{
	png::image< png::rgba_pixel > image(filename);
	png::pixel_buffer<png::rgba_pixel> img_buf = image.get_pixbuf();
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			texture[(height-1 - i) * width * 4 + j * 4 + 0] = img_buf.get_pixel(j, i).red;
			texture[(height-1 - i) * width * 4 + j * 4 + 1] = img_buf.get_pixel(j, i).green;
			texture[(height-1 - i) * width * 4 + j * 4 + 2] = img_buf.get_pixel(j, i).blue;
			texture[(height-1 - i) * width * 4 + j * 4 + 3] = img_buf.get_pixel(j, i).alpha;
		}
	}

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}
