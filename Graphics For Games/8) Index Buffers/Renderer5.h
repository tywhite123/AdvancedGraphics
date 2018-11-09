#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include "..\..\nclgl\OBJMesh.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\SceneNode.h"
#include"..\..\nclgl\Frustum.h"
#include "..\..\nclgl\HeightMap.h"
#include "..\..\nclgl\TextMesh.h"
#include <algorithm>

class Renderer6 : public OGLRenderer
{
public:
	Renderer6(Window &parent);
	virtual ~Renderer6();

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void	DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void DrawSkybox();

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

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	float time;
	
	GLuint cubeMap;

	Font* basicFont;
	Shader* fontShader;
	

	float start;
	float end;
	float timeSinceProfile;
	int framerate;
	Window* w;
	string fps;
	string rTime;
	
};

