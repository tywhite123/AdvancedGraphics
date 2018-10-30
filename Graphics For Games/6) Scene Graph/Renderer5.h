#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\SceneNode.h"
#include"..\..\nclgl\Frustum.h"
#include "CubeRobot.h"
#include <algorithm>

class Renderer6 : public OGLRenderer
{
public:
	Renderer6(Window &parent);
	virtual ~Renderer6();

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	SceneNode* root;
	Camera* camera;
	Mesh* quad;

	Shader* shader;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
	
	Frustum frameFrustum;
};

