#pragma once

#include "Matrix4.h"
#include "Mesh.h"
#include "Shader.h"

class SceneNode
{
public:
	SceneNode(void);
	SceneNode(Mesh* m, Shader* s, GLuint t = 0);
	SceneNode(Mesh* m, Shader* s, Vector4 c = Vector4(1,1,1,1));
	~SceneNode();

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh*m) { mesh = m; }

	Shader* GetShader() const { return shader; }
	void SetShader(Shader* s) { shader = s; }

	Vector4 GetColour()const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Matrix4 GetModelMatrix() const { return modelMatrix; }
	void SetModelMatrix(Matrix4 mat) { modelMatrix = mat; }

	Vector3 GetScale() const { return modelScale; }
	void SetScale(Vector3 s) { modelScale = s; }

	Matrix4 GetWorldTransform() const { return worldTransform; }

	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

	void AddChild(SceneNode* s);

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer &r);

	vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

protected:
	Mesh* mesh;
	Shader* shader;

	GLuint tex;

	Matrix4 modelMatrix;
	Matrix4 worldTransform;

	Vector3 modelScale;
	Vector4 colour;

	float distanceFromCamera;
	float boundingRadius;

	SceneNode* parent;
	vector<SceneNode*> children;
};

