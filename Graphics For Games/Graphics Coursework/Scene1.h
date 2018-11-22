#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\HeightMap.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\ParticleBehaviour.h"
#include "..\..\nclgl\HeightMapFromJPG.h"
class Scene1 :
	public SceneNode
{
public:
	Scene1();
	~Scene1();

	GLuint GetHeightMapTex() { return hmap->GetTexture(); }
	GLuint GetHeightBumpMap() { return hmap->GetBumpMap(); }
	GLuint GetWaterTex() { return water->GetTexture(); }


protected:
	HeightMapFromJPG* hmap;
	HeightMap* water;
	Mesh* spyro;

	Shader* terrainShader;
	Shader* waterShader;
	Shader* spyroShader;

};

