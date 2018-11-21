#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include "..\..\nclgl\OBJMesh.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\SceneNode.h"
#include"..\..\nclgl\Frustum.h"
#include "..\..\nclgl\HeightMap.h"
#include "..\..\nclgl\TextMesh.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "..\..\nclgl\ParticleBehaviour.h"
#include <algorithm>


#define SHADOWSIZE 2048
#define POST_PASSES 10

enum PostType {
	BLUR = 0, SOBEL = 1, BLOOM = 2, INVERT = 3, GRAYSCALE = 4, STATIC = 5
};

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	virtual ~Renderer();

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void	DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:

	//RENDER FUNCTIONS
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void DrawSkybox();
	void DrawScene();
	void DrawParticleSystem();
	void DrawShadows();
	void DrawPostProcess();
	void PresentScene();
	void DrawUI();
	float partEnd;

	//FBO SETUPS
	void SetupShadows();
	void SetupScreen();
	void SetupPostProcess();
	void SetupDeferred();
	//ADD IN GEN SCREEN TEX FROM DEFERRED
	void GenerateScreenTextures(GLuint & into, bool depth);

	//DEFFERED STUFF?
	void FillBuffers();
	void DrawLights();
	void CombineBuffers();

	//VARIABLES
	SceneNode* root;
	SceneNode* root2;
	SceneNode* root3;
	ParticleBehaviour* particles;
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
	Shader* screenShader;
	Shader* processShader;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	float time;
	
	GLuint cubeMap[3];
	GLuint grass;

	Font* basicFont;
	Shader* fontShader;
	

	float start;
	float end;
	float testEnd;
	float timeSinceProfile;
	float timeSinceSwitch;
	bool switching;
	int framerate;
	Window* w;
	string fps;
	string rTime;

	int scene;
	bool rain;
	bool profiler;
	
	Scene1* scene1;
	Scene2* scene2;
	Scene3* scene3;

	bool shadows;
	bool drawingShadows;
	GLuint shadowTex;
	GLuint shadowFBO;
	Shader* shadowShader;
	Matrix4 shadowMatrix;

	Shader* lightShader;
	Shader* pointShader;

	GLuint sceneFBO;
	GLuint sceneColourTex[3];
	GLuint sceneDepthTex;

	bool post;
	PostType pType;
	GLuint processFBO;

	GLuint deferredFBO;
	GLuint lightFBO;
};

