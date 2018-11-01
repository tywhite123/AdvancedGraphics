#pragma once

#include "..\..\nclgl\OGLRenderer.h"
#include "..\..\nclgl\Camera.h"
#include "..\..\nclgl\MD5Mesh.h"

class Renderer9 : public OGLRenderer
{
public:
	Renderer9(Window &parent);
	virtual ~Renderer9();

	virtual void RenderScene();
	virtual void UpdateScene(float msec);


protected:
	MD5FileData* hellData;
	MD5Node* hellNode;
	Camera* camera;

};

