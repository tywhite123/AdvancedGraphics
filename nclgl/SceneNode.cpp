#include "SceneNode.h"



SceneNode::SceneNode()
{
	mesh = NULL;
	shader = NULL;
	tex = NULL;
	parent = NULL;
	colour = Vector4(1, 1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	isSkeletal = false;
}

SceneNode::SceneNode(Mesh * m, Shader * s, GLuint t)
{
	mesh = m;
	shader = s;
	tex = t;
	parent = NULL;
	colour = Vector4(1, 1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	isSkeletal = false;
}

SceneNode::SceneNode(Mesh * m, Shader * s, Vector4 c)
{
	mesh = m;
	shader = s;
	tex = NULL;
	parent = NULL;
	colour = c;
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	isSkeletal = false;
}


SceneNode::~SceneNode()
{
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode * s)
{
	children.push_back(s);
	s->parent = this;
}

void SceneNode::Update(float msec)
{

	//if there is a parent set relative to the parents world transform
	if (parent)
		worldTransform = parent->worldTransform * modelMatrix;
	else
		worldTransform = modelMatrix;

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i)
		(*i)->Update(msec);
}

void SceneNode::Draw(const OGLRenderer & r)
{
	if (mesh) { mesh->Draw();}
}
