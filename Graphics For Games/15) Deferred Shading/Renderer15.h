#pragma once

#include"..\..\nclgl\OGLRenderer.h"
#include"..\..\nclgl\Camera.h"
#include"..\..\nclgl\OBJMesh.h"
#include"..\..\nclgl\HeightMap.h"

#define LIGHTNUM 8

class Renderer15 : public OGLRenderer
{
public:
	Renderer15(Window &parent);
	virtual ~Renderer15();


	virtual void UpdateScene(float msec);
	virtual void RenderScene();

protected:

	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();
	void GenerateScreenTexture(GLuint &into, bool depth = false);

	Shader* sceneShader; //Fills GBuffers
	Shader* pointLightShader; //Calculates lighting
	Shader* combineShader; //All toghether

	Light* pointLights;
	Mesh* heightMap;
	OBJMesh* sphere;
	Mesh* quad;
	Camera* camera;

	float rotation; //?

	GLuint bufferFBO;
	GLuint bufferColourTex;
	GLuint bufferNormalTex;
	GLuint bufferDepthTex;

	GLuint pointLightFBO;
	GLuint lightEmissiveTex;
	GLuint lightSpecularTex;


};

