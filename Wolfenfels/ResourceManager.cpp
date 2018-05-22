#include "ResourceManager.h"

vector<BasicTexture*> ResourceManager::m_textures;
vector<Model*> ResourceManager::m_models;
array<GLfloat, 12> ResourceManager::m_enemyMesh = {
	-0.5f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.9f,
	-0.5f, 0.0f, 0.9f
};

array<GLfloat, 12> ResourceManager::m_playerMesh = {
	-0.27f, -1.0f, -1.0f,
	0.13f, -1.0f, -1.0f,
	0.13f, 0.1f, -1.0f,
	-0.27f, 0.1f, -1.0f
};

vector<GLfloat> ResourceManager::m_wallMesh;
array<GLfloat, 24> ResourceManager::m_bgMesh;



void ResourceManager::addTexture(std::string filename)
{
	BasicTexture* texture = new BasicTexture(filename);
	m_textures.push_back(texture);
}

Model* ResourceManager::addPlayerModel()
{
	Model* model = new Model();
	model->genVAO();
	model->addVBO(0, 3, m_playerMesh.size(), m_playerMesh.data());

	array<GLfloat, 12> placeholderUV = {
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};

	model->addVBO(1, 2, placeholderUV.size(), placeholderUV.data());
	model->m_texture = m_textures[1];

	m_models.push_back(model);
	return model;
}

Model* ResourceManager::addWallModel()
{
	Model* model = new Model();
	model->genVAO();
	model->addVBO(0, 3, m_wallMesh.size(), m_wallMesh.data());

	array<GLfloat, 12> placeholderUV = {
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};

	model->addVBO(1, 2, placeholderUV.size(), placeholderUV.data());
	model->m_texture = m_textures[0];

	m_models.push_back(model);
	return model;
}

Model * ResourceManager::addBgModel()
{
	Model* model = new Model();
	model->genVAO();
	model->addVBO(0, 3, m_bgMesh.size(), m_bgMesh.data());

	array<GLfloat, 12> placeholderColor = {
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};

	model->addVBO(2, 3, placeholderColor.size(), placeholderColor.data());

	m_models.push_back(model);
	return model;
}

Model* ResourceManager::addEnemyModel()
{
	Model* model = new Model();
	model->genVAO();
	model->addVBO(0, 3, m_enemyMesh.size(), m_enemyMesh.data());

	array<GLfloat, 12> placeholderUV = {
		0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};

	model->addVBO(1, 2, placeholderUV.size(), placeholderUV.data());
	model->m_texture = m_textures[2];

	m_models.push_back(model);
	return model;
}

void ResourceManager::load()
{
	addTexture("Assets/wall.png");
	addTexture("Assets/pistol.png");
	addTexture("Assets/Cyclops.png");
}

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}
