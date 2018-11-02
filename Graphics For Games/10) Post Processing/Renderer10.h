#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include"..\..\nclgl\HeightMap.h"
#include "..\..\nclgl\Camera.h"

#define POST_PASSES 1

class Renderer10 : public OGLRenderer
{
public:
	Renderer10(Window &parent);
	virtual ~Renderer10();
	
	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void PresentScene();
	void DrawPostProcess();
	void DrawScene();

	Shader* sceneShader;
	Shader* processShader;

	Camera* camera;
	Camera* camera2;
	Camera* camera3;
	Camera* camera4;

	Mesh* quad;
	HeightMap* heightMap;

	GLuint bufferFBO;
	GLuint bufferFBO2;
	GLuint processFBO;
	GLuint bufferFBO3;
	GLuint bufferFBO4;
	GLuint bufferColourTex[8];
	GLuint bufferDepthTex;





};

