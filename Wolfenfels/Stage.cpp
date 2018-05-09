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
