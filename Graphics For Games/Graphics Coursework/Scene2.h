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

	GLuint GetHeightMapTex() { return heightMap->GetTexture(); }
	GLuint GetHeightBumpMap() { return heightMap->GetBumpMap(); }


protected:

	Mesh* winterTundra;
	HeightMap* heightMap;
	Mesh* quad;

	Shader* terrainShader;
	Shader* waterShader;
	Shader* spyroShader;
	Shader* skyboxShader;
};