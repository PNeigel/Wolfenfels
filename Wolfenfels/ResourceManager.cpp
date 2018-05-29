#include "ResourceManager.h"
#include "Utils.h"
#include <iostream>

vector<BasicTexture*> ResourceManager::m_textures;
vector<Model*> ResourceManager::m_models;
array<GLfloat, 12> ResourceManager::m_enemyMesh = {
	-0.5f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.9f,
	-0.5f, 0.0f, 0.9f
};
array<GLfloat, 24*3> ResourceManager::m_doorMeshDict = {
	-0.5f, -0.5f, 0.0f,
	-0.4f, -0.5f, 0.0f,
	-0.4f, -0.5f, 1.0f,
	-0.5f, -0.5f, 1.0f,
	-0.4f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 1.0f,
	-0.4f, 0.5f, 1.0f, //
	0.4f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 1.0f,
	0.4f, -0.5f, 1.0f,
	0.5f, 0.5f, 0.0f,
	0.4f, 0.5f, 0.0f,
	0.4f, 0.5f, 1.0f,
	0.5f, 0.5f, 1.0f, //
	-0.4f, -0.1f, 0.0f,
	0.4f, -0.1f, 0.0f,
	0.4f, -0.1f, 1.0f,
	-0.4f, -0.1f, 1.0f,
	0.4f, 0.1f, 0.0f,
	-0.4f, 0.1f, 0.0f,
	-0.4f, 0.1f, 1.0f,
	0.4f, 0.1f, 1.0f
};

vector<GLfloat> ResourceManager::m_doorMesh;
vector<GLfloat> ResourceManager::m_doorUV;
vector<GLfloat> ResourceManager::m_doorAnimAffected;

vector<GLfloat> ResourceManager::m_doorUVDict = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};
array<GLfloat, 12> ResourceManager::m_playerMesh;

vector<GLfloat> ResourceManager::m_wallMesh;
array<GLfloat, 24> ResourceManager::m_bgMesh;



void ResourceManager::addTexture(std::string filename)
{
	BasicTexture* texture = new BasicTexture(filename);
	m_textures.push_back(texture);
}

void ResourceManager::addTextureAtlas(std::string filename, int texPerRow, int texPerCol)
{
	TextureAtlas* texture = new TextureAtlas(filename, texPerRow, texPerCol);
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

Model * ResourceManager::addDoorModel()
{
 	static Model* doorModel;

	if (doorModel != nullptr)
		return doorModel;

	Model* model = new Model();
	model->genVAO();


	vector<int> indices = {
		0, 1, 2, 3,
		1, 4, 7, 2,
		4, 5, 6, 7,
		5, 0, 3, 6,
		8, 9, 10, 11,
		9, 12, 15, 10,
		12, 13, 14, 15,
		13, 8, 11, 14,
		16, 17, 18, 19,
		17, 20, 23, 18,
		20, 21, 22, 23,
		21, 16, 19, 22
	};
	m_doorMesh = BufferFromIndex(m_doorMeshDict.data(), m_doorMeshDict.size(), 3, indices.data(), indices.size());

	model->addVBO(0, 3, m_doorMesh.size(), m_doorMesh.data());

	vector<int> indicesUV;

	for (size_t i = 0; i < indices.size() / 4; i++) {
		indicesUV.push_back(0);
		indicesUV.push_back(1);
		indicesUV.push_back(2);
		indicesUV.push_back(3);
	}

	m_doorUV = BufferFromIndex(m_doorUVDict.data(), m_doorUVDict.size(), 2, indicesUV.data(), indicesUV.size());
	model->addVBO(1, 2, m_doorUV.size(), m_doorUV.data());
	model->m_texture = m_textures[3];

	for (int i = 0; i < indices.size(); i++) {
		if (i < indices.size() / 3)
			m_doorAnimAffected.push_back(0);
		else
			m_doorAnimAffected.push_back(1);
	}

	model->addVBO(3, 1, m_doorAnimAffected.size(), m_doorAnimAffected.data());

	m_models.push_back(model);

	doorModel = model;
	return model;
}

void ResourceManager::load()
{
	addTexture("Assets/wallblue.png");
	addTextureAtlas("Assets/pistol.png", 5, 1);
	addTextureAtlas("Assets/Cyclops.png", 2, 1);
	addTexture("Assets/wall.png");
}

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}
