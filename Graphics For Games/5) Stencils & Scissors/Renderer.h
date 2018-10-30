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

	void ToggleObject();
	void ToggleDepth();
	void ToggleAlphaBlend();
	void ToggleBlendMode();
	void MoveObject(float by);

	void ToggleScissor();
	void ToggleStencil();

protected:
	GLuint textures[2];
	Mesh* meshes[2];
	Vector3 positions[2];


	bool usingScissor;
	bool usingStencil;

	Matrix4 textureMatrix;
	Matrix4 modelMatrix;
	Matrix4 projMatrix;
	Matrix4 viewMatrix;

	bool modifyObject;
	bool usingDepth;
	bool usingAlpha;
	int blendMode;
	
	Mesh* triangle;
	Camera* camera;

	bool filtering;
	bool repeating;

	float scale;
	float rotation;
	Vector3 position;
};
