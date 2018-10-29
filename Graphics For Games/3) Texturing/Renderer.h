#pragma once

#include "..\..\nclgl\Camera.h"
#include"..\..\nclgl\OGLRenderer.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);
	
	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void SwitchToPerspective();
	void SwitchToOrthographic();


	inline void SetScale(float s)		{ scale = s; }
	inline void SetRotation(float r)	{ rotation = r; }
	inline void SetPosition(Vector3 p)	{ position = p; }

	void UpdateTextureMatrix(float rotation);
	void ToggleRepeating();
	void ToggleFiltering();

protected:
	Mesh* triangle;
	Camera* camera;

	bool filtering;
	bool repeating;

	float scale;
	float rotation;
	Vector3 position;
};
