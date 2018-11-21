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

	virtual void Update(float msec);



protected:
	Shader* sceneShader;
	Shader* hellKnightSceneShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	Mesh* floor;
	Mesh* spyro;
	Light* light;
};

