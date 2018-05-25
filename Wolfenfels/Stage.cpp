#include "Stage.h"

#include <fstream>
#include <sstream>

bool CloseDistance(const Wall& wall, glm::vec2 objectPos, float threshold)
{

	return (glm::length(wall.sides[0] - objectPos) <= threshold);
}


bool WallDistCompare(const Wall& wallA, const Wall& wallB, glm::vec2 target)
{
	// this wall
	glm::vec2 diffA = glm::vec2(target) - wallA.sides[0];
	float distA = glm::abs(glm::dot(diffA, wallA.m_normal));
	// other wall
	glm::vec2 diffB = glm::vec2(target) - wallB.sides[0];
	float distB = glm::abs(glm::dot(diffB, wallB.m_normal));

	if (glm::abs(distA - distB) < 0.001) {
		distA = glm::min(glm::length(diffA), glm::length(glm::vec2(target) - wallA.sides[1]));
		distB = glm::min(glm::length(diffB), glm::length(glm::vec2(target) - wallB.sides[1]));
	}

	return (distA < distB);
}


Stage::Stage(int stage_no, Player & player)
{
	stringstream ssfilename;
	ssfilename << "Stages/stage" << std::to_string(stage_no) << ".txt";
	string filename = ssfilename.str();
	ReadStageFromPNG("Stages/stage.png");
	std::sort(walls.begin(), walls.end(),
		[&](const Wall& wallA, const Wall& wallB) {return WallDistCompare(wallA, wallB, glm::vec2(player.pos));}
	);
	SetWallVerts();
	SetBGVerts();
	m_wallModel = ResourceManager::addWallModel();
	m_wallModel->updateVBO(1, wall_UV_coords.size(), wall_UV_coords.data());
	m_bgModel = ResourceManager::addBgModel();
	m_bgModel->updateVBO(1, bgcolors.size(), bgcolors.data());
	ResourceManager::addEnemyModel();
	for (int i = 0; i < 2; i++) {
		enemies.push_back(Enemy(glm::vec3{ 2*i+3, 2*i+3, 0.0 }, player));
	}
	enemies.push_back(Enemy(glm::vec3{ 4.5 , 6.5, 0.0 }, player));
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

void Stage::ReadStageFromPNG(string filename)
{
	png::image< png::rgba_pixel > image(filename);
	png::pixel_buffer<png::rgba_pixel> img_buf = image.get_pixbuf();
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
						Wall wall = Wall(glm::vec2{ j, height-i }, glm::vec2{ j + 1, height - i });
						walls.push_back(wall);
					}
				}
				else {
					Wall wall = Wall(glm::vec2{ j, height - i }, glm::vec2{ j + 1, height - i });
					walls.push_back(wall);
				}
				// check right
				if (j < width-1) {
					r = img_buf.get_pixel(j+1, i).red;
					g = img_buf.get_pixel(j+1, i).green;
					b = img_buf.get_pixel(j+1, i).blue;
					a = img_buf.get_pixel(j+1, i).alpha;
					if (r == 0 && g == 0 && b == 0) {
						Wall wall = Wall(glm::vec2{ j + 1, height - i }, glm::vec2{ j + 1, height - (i + 1) });
						walls.push_back(wall);
					}
				}
				else {
					Wall wall = Wall(glm::vec2{ j + 1, height - i }, glm::vec2{ j + 1, height - (i + 1) });
					walls.push_back(wall);
				}
				// check bottom
				if (i < height-1) {
					r = img_buf.get_pixel(j, i + 1).red;
					g = img_buf.get_pixel(j, i + 1).green;
					b = img_buf.get_pixel(j, i + 1).blue;
					a = img_buf.get_pixel(j, i + 1).alpha;
					if (r == 0 && g == 0 && b == 0) {
						Wall wall = Wall(glm::vec2{ j + 1, height - (i + 1) }, glm::vec2{ j, height - (i + 1) });
						walls.push_back(wall);
					}
				}
				else {
					Wall wall = Wall(glm::vec2{ j + 1, height - (i + 1) }, glm::vec2{ j, height - (i + 1) });
					walls.push_back(wall);
				}
				// check left
				if (j > 0) {
					r = img_buf.get_pixel(j - 1, i).red;
					g = img_buf.get_pixel(j - 1, i).green;
					b = img_buf.get_pixel(j - 1, i).blue;
					a = img_buf.get_pixel(j - 1, i).alpha;
					if (r == 0 && g == 0 && b == 0) {
						Wall wall = Wall(glm::vec2{ j, height - (i + 1) }, glm::vec2{ j, height - i });
						walls.push_back(wall);
					}
				}
				else {
					Wall wall = Wall(glm::vec2{j, height - (i + 1) }, glm::vec2{ j, height - i });
					walls.push_back(wall);
				}
			}
		}
	}
}

void Stage::Tick(double delta_time, Player & player)
{
	auto firstNotClose = std::partition(walls.begin(), walls.end(),
		[&](Wall& wall) {return CloseDistance(wall, glm::vec2(player.pos), m_closeDistance);}
	);
	std::sort(walls.begin(), firstNotClose,
		[&](const Wall& wallA, const Wall& wallB) {return WallDistCompare(wallA, wallB, glm::vec2(player.pos));}
	);
	std::sort(enemies.begin(), enemies.end());
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
