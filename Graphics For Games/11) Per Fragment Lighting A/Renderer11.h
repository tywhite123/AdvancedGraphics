#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\HeightMap.h"

class Renderer11 : public OGLRenderer
{
public:
	Renderer11(Window &parent);
	virtual ~Renderer11();

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	Mesh* heightMap;
	Camera* camera;
	Light* light;
};

