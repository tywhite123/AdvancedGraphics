#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\MD5Mesh.h"
#include "..\..\nclgl\MD5Node.h"
class Scene3 :
	public SceneNode
{
public:
	Scene3();
	~Scene3();

	GLuint GetFloorTex() { return floor->GetTexture(); }


protected:
	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	Mesh* floor;
	Mesh* spyro;
	Light* light;
};

