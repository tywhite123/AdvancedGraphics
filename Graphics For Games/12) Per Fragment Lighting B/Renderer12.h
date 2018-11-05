#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\HeightMap.h"

class Renderer12 : public OGLRenderer
{
public:
	Renderer12(Window &parent);
	virtual ~Renderer12();

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	Mesh* heightMap;
	Camera* camera;
	Light* light;
};

