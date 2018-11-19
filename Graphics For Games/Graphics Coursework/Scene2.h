#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\HeightMap.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\ParticleBehaviour.h"
#include "..\..\nclgl\HeightMapFromJPG.h"
class Scene2 :
	public SceneNode
{
public:
	Scene2();
	~Scene2();

	GLuint GetHeightMapTex() { return hmap->GetTexture(); }
	GLuint GetHeightBumpMap() { return hmap->GetBumpMap(); }
	GLuint GetWaterTex() { return water->GetTexture(); }
	GLuint GetCubeMap() { return cubeMap; }


protected:

	Mesh* winterTundra;
	SceneNode* root;
	Camera* camera;
	HeightMap* heightMap;
	HeightMapFromJPG* hmap;
	HeightMap* water;
	Mesh* spyro;
	Mesh* quad;
	Light* light;

	Shader* terrainShader;
	Shader* waterShader;
	Shader* spyroShader;
	Shader* skyboxShader;
	Shader* particleShader;

	GLuint cubeMap;
};