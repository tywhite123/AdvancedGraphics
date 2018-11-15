#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\HeightMap.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\ParticleBehaviour.h"
class Scene1 :
	public SceneNode
{
public:
	Scene1();
	~Scene1();

	GLuint GetHeightMapTex() { return heightMap->GetTexture(); }
	GLuint GetHeightBumpMap() { return heightMap->GetBumpMap(); }
	GLuint GetWaterTex() { return water->GetTexture(); }
	GLuint GetCubeMap() { return cubeMap; }


protected:
	SceneNode* root;
	Camera* camera;
	HeightMap* heightMap;
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

