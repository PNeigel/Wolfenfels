#include "Stage.h"

#include <fstream>
#include <sstream>


Stage::Stage(int stage_no)
{
	stringstream ssfilename;
	ssfilename << "Stages/stage" << std::to_string(stage_no) << ".txt";
	string filename = ssfilename.str();
	ReadStageFromPNG("Stages/stage.png");
	SetWallVerts();
	SetBGVerts();
	m_wallModel = ResourceManager::addWallModel();
	m_wallModel->updateVBO(1, wall_UV_coords.size(), wall_UV_coords.data());
	m_bgModel = ResourceManager::addBgModel();
	m_bgModel->updateVBO(1, bgcolors.size(), bgcolors.data());
	ResourceManager::addEnemyModel();
	enemies.push_back(Enemy(glm::vec3{4.0, 4.0, 0.0}));
	enemies.push_back(Enemy(glm::vec3{ 3.0, 3.0, 0.0 }));
}


Stage::~Stage()
{
}

void Stage::SetWallVerts()
{	// Create the vertex positions for the VBO's
	for (uint16_t i = 0; i < walls.size(); i++) { // Set vertices for every wall. Top is z = 1.0, bottom z = 0.0
		ResourceManager::m_wallMesh.push_back(walls[i].sides[0].x);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[0].y);
		ResourceManager::m_wallMesh.push_back(0.0f);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[1].x);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[1].y);
		ResourceManager::m_wallMesh.push_back(0.0f);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[1].x);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[1].y);
		ResourceManager::m_wallMesh.push_back(1.0f);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[0].x);
		ResourceManager::m_wallMesh.push_back(walls[i].sides[0].y);
		ResourceManager::m_wallMesh.push_back(1.0f);
	}

	float UV_dict[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	for (uint16_t i = 0; i < walls.size(); i++) { // Set colors for every wall.
		for (int j = 0; j < 8; j++) { // 12 is the number of components per quad (3*4)
			wall_UV_coords.push_back(UV_dict[j]);
		}
	}
}

void Stage::SetBGVerts()
{
	ResourceManager::m_bgMesh = {
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
	bgcolors = {
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
}

void Stage::initBgVBOs()
{
	glGenBuffers(1, &m_bgVertVBO); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_bgVertVBO); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), bgverts.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_bgColorVBO); // Generate empty buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_bgColorVBO); // Bind as current buffer in OpenGL's state machine
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), bgcolors.data(), GL_STATIC_DRAW);
}

void Stage::initBgVAO()
{
	glGenVertexArrays(1, &m_bgVAO);
	glBindVertexArray(m_bgVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_bgVertVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_bgColorVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Stage::ReadStageFromPNG(string filename)
{
	png::image< png::rgba_pixel > image(filename);
	png::pixel_buffer<png::rgba_pixel> img_buf = image.get_pixbuf();
	Wall wall;
	int width = image.get_width();
	int height = image.get_height();
	unsigned char r, g, b, a;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			r = img_buf.get_pixel(j, i).red;
			g = img_buf.get_pixel(j, i).green;
			b = img_buf.get_pixel(j, i).blue;
			a = img_buf.get_pixel(j, i).alpha;
			if (r == 255 && g == 255 && b == 255) { // if pixel is white
				// check 4 neighboring pixels
				// check top
				if (i > 0) {
					r = img_buf.get_pixel(j, i-1).red;
					g = img_buf.get_pixel(j, i-1).green;
					b = img_buf.get_pixel(j, i-1).blue;
					a = img_buf.get_pixel(j, i-1).alpha;
					if (r == 0 && g == 0 && b == 0) {
						wall = Wall(glm::vec2{ j, height-i }, glm::vec2{ j + 1, height - i });
						walls.push_back(wall);
					}
				}
				else {
					wall = Wall(glm::vec2{ j, height - i }, glm::vec2{ j + 1, height - i });
					walls.push_back(wall);
				}
				// check right
				if (j < width-1) {
					r = img_buf.get_pixel(j+1, i).red;
					g = img_buf.get_pixel(j+1, i).green;
					b = img_buf.get_pixel(j+1, i).blue;
					a = img_buf.get_pixel(j+1, i).alpha;
					if (r == 0 && g == 0 && b == 0) {
						wall = Wall(glm::vec2{ j + 1, height - i }, glm::vec2{ j + 1, height - (i + 1) });
						walls.push_back(wall);
					}
				}
				else {
					wall = Wall(glm::vec2{ j + 1, height - i }, glm::vec2{ j + 1, height - (i + 1) });
					walls.push_back(wall);
				}
				// check bottom
				if (i < height-1) {
					r = img_buf.get_pixel(j, i + 1).red;
					g = img_buf.get_pixel(j, i + 1).green;
					b = img_buf.get_pixel(j, i + 1).blue;
					a = img_buf.get_pixel(j, i + 1).alpha;
					if (r == 0 && g == 0 && b == 0) {
						wall = Wall(glm::vec2{ j + 1, height - (i + 1) }, glm::vec2{ j, height - (i + 1) });
						walls.push_back(wall);
					}
				}
				else {
					wall = Wall(glm::vec2{ j + 1, height - (i + 1) }, glm::vec2{ j, height - (i + 1) });
					walls.push_back(wall);
				}
				// check left
				if (j > 0) {
					r = img_buf.get_pixel(j - 1, i).red;
					g = img_buf.get_pixel(j - 1, i).green;
					b = img_buf.get_pixel(j - 1, i).blue;
					a = img_buf.get_pixel(j - 1, i).alpha;
					if (r == 0 && g == 0 && b == 0) {
						wall = Wall(glm::vec2{ j, height - (i + 1) }, glm::vec2{ j, height - i });
						walls.push_back(wall);
					}
				}
				else {
					wall = Wall(glm::vec2{j, height - (i + 1) }, glm::vec2{ j, height - i });
					walls.push_back(wall);
				}
			}
		}
	}
}

void Stage::Tick(double delta_time, Player & player)
{
	if (enemies.size() > 0) {
		for (int i = 0; i < enemies.size();) {
			enemies[i].Tick(delta_time, player, *this);
			if (enemies[i].current_hp <= 0) {
				enemies.erase(enemies.begin() + i);
			}
			else i++;
		}
	}
}
