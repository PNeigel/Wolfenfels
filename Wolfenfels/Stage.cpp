#include "Stage.h"

#include <fstream>
#include <sstream>

/**
	Returns true if wall is closer to objectPos than threshold.
**/
bool CloseDistance(const Wall& wall, glm::vec2 objectPos, float threshold)
{
	return (glm::length(wall.sides[0] - objectPos) <= threshold);
}


/**
	Returns true if wallA is closer to target than wallB.
	The wall with the start- or end-point that is closest to target is regarded as the closer wall.
**/
bool WallDistCompare(const Wall& wallA, const Wall& wallB, glm::vec2 target)
{
	float leftDistA = glm::length(glm::vec2(target) - wallA.sides[0]);
	float rightDistA = glm::length(glm::vec2(target) - wallA.sides[1]);
	float leftDistB = glm::length(glm::vec2(target) - wallB.sides[0]);
	float rightDistB = glm::length(glm::vec2(target) - wallB.sides[1]);

	float distA = glm::min(leftDistA, rightDistA);
	float distB = glm::min(leftDistB, rightDistB);

	return (distA < distB);
}
/** 
	Returns true if posA is closer to targetPos than posB
**/
bool CloserToThan(glm::vec3 posA, glm::vec3 posB, glm::vec3 targetPos)
{
	return (glm::length(posA - targetPos) < glm::length(posB - targetPos));
}

bool operator==(const png::rgb_pixel & pixA, const png::rgb_pixel & pixB) {
	if ((int)pixA.red != (int)pixB.red || (int)pixA.green != (int)pixB.green
		|| (int)pixA.blue != (int)pixB.blue)
		return false;
	else return true;
}
bool operator!=(const png::rgb_pixel & pixA, const png::rgb_pixel & pixB) {
	return !(pixA == pixB);
}


Stage::Stage(int stage_no)
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

	const png::rgb_pixel whiteColor(255, 255, 255);
	const png::rgb_pixel wallColor(0, 0, 0);
	const png::rgb_pixel doorColor(100, 100, 255);
	const png::rgb_pixel enemyColor(255, 50, 100);
	const png::rgb_pixel playerColor(0, 200, 0);

	png::image< png::rgb_pixel > image(filename);
	png::pixel_buffer<png::rgb_pixel> img_buf = image.get_pixbuf();
	
	int width = image.get_width();
	int height = image.get_height();

	bool playerSet = false;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {

			png::rgb_pixel current, top, right, bottom, left;

			current = img_buf[i][j];
			// we consider all "out of bounds" pixels as black
			if (i > 0)
				top = img_buf[i-1][j];
			if (j < width -1)
				right = img_buf[i][j+1];
			if (i < height -1)
				bottom = img_buf[i+1][j];
			if (j > 0)
				left = img_buf[i][j-1];

			// Wall setup
			if ( current != wallColor ) { // if pixel is not black
				// check 4 neighboring pixels
				// check top
				if ( top == wallColor ) {
					Wall wall = Wall(glm::vec2{ j, height-i }, glm::vec2{ j + 1, height - i });
					walls.push_back(wall);
				}
				// check right
				if ( right == wallColor ) {
					Wall wall = Wall(glm::vec2{ j + 1, height - i }, glm::vec2{ j + 1, height - (i + 1) });
					walls.push_back(wall);
				}
				// check bottom
				if ( bottom == wallColor ) {
					Wall wall = Wall(glm::vec2{ j + 1, height - (i + 1) }, glm::vec2{ j, height - (i + 1) });
					walls.push_back(wall);
				}
				// check left
				if ( left == wallColor ) {
					Wall wall = Wall(glm::vec2{ j, height - (i + 1) }, glm::vec2{ j, height - i });
					walls.push_back(wall);
				}
			}

			// Door setup

			if ( current == doorColor ) { // pixel is marked as door
				// check 4 neighboring pixels
				// check top and bottom
				if ( top == wallColor || bottom == wallColor ) { // or bottom black?
					Door door(glm::vec3{ j + 0.5, height - 1 -i + 0.5, 0.0f }, true);
					m_doors.push_back(door);
					continue;
				}
				// check right and left
				if ( right == wallColor || left == wallColor) { // or left black?
					Door door(glm::vec3{ j + 0.5, height - 1 - i + 0.5, 0.0f }, false);
					m_doors.push_back(door);
					continue;
				}
			}

			// Enemy setup

			else if (current == enemyColor) {
					enemies.push_back(Enemy(glm::vec3{ j+0.5, height-1-i+0.5, 0.0 }));
			}

			// Player setup

			if (!playerSet && current == playerColor) {
				player = Player(glm::vec3{j+0.5, height-1-i+0.5, 0.5f});
				playerSet = true;
			}
		}
	}
}

void Stage::Tick(double delta_time, CollisionHandler & coll, int* keystates)
{
	player.Update(delta_time, coll, *this, keystates);

	// Partition all walls in "close" (distance > m_closeDistance) and "not close".
	// Return iterator of first wall that is "not close".
	auto firstNotClose = std::partition(walls.begin(), walls.end(),
		[&](Wall& wall) {return CloseDistance(wall, glm::vec2(player.pos), m_closeDistance);}
	);

	// Sort the walls that are close according to their distance to the player.
	std::sort(walls.begin(), firstNotClose,
		[&](const Wall& wallA, const Wall& wallB) {return WallDistCompare(wallA, wallB, glm::vec2(player.pos));}
	);

	// Sort the doors according to the distance to the player.
	std::sort(m_doors.begin(), m_doors.end(),
		[&](const Door& doorA, const Door& doorB) {return CloserToThan(doorA.m_pos, doorB.m_pos, player.pos);}
	);

	// Sort the enemies according to the distance to the player.
	std::sort(enemies.begin(), enemies.end(),
		[&](const Enemy& enemyA, const Enemy& enemyB) {return CloserToThan(enemyA.pos, enemyB.pos, player.pos);}
	);

	// Loop over enemies: Tick enemy and kill + delete it if HP < 0.
	if (enemies.size() > 0) {
		for (int i = 0; i < enemies.size();) {
			enemies[i].Tick(delta_time, player, *this);
			if (enemies[i].current_hp <= 0) {
				enemies.erase(enemies.begin() + i);
			}
			else i++;
		}
	}

	// Loop over doors: Tick door.
	for (Door& door : m_doors)
		door.tick(delta_time);
}
