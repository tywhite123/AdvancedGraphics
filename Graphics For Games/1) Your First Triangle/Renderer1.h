#pragma once

#include "..\..\nclgl\OGLRenderer.h"

class Renderer1 : public OGLRenderer
{
public:
	Renderer1(Window &parent);
	virtual ~Renderer1(void);
	virtual void RenderScene();

protected:
	Mesh* triangle;


};

