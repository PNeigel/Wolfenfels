#pragma once

#include "Texture/TextureAtlas.h"
#include "Model.h"

#include <vector>
#include <array>
#include <string>

class ResourceManager
{
	public:
		~ResourceManager();

		static vector<BasicTexture*> m_textures;
		static vector<Model*> m_models;

		static void addTexture(std::string filename);
		static void addTextureAtlas(std::string filename, int texPerRow, int texPerCol);
		static Model* addPlayerModel();
		static Model* addWallModel();
		static Model* addBgModel();
		static Model* addEnemyModel();

		static void load();

		static array<GLfloat, 12> m_enemyMesh;
		static array<GLfloat, 12> m_playerMesh;
		static vector<GLfloat> m_wallMesh;
		static array<GLfloat, 24> m_bgMesh;

	private:
		ResourceManager();
};

