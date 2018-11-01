#include "Renderer5.h"



Renderer6::Renderer6(Window & parent) : OGLRenderer(parent)
{
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");

	projMatrix = Matrix4::Perspective(1.0f, 1000000.0f, (float)width / (float)height, 45.0f);
	
	camera = new Camera(-40,270, Vector3(-2000.0f, 3300.0f, 2000.0f));
	root = new SceneNode();
	

	//camera->SetPosition(Vector3(0, 30, 175));
	
	
	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!heightMap->GetTexture())
		return;

	SetTextureRepeating(heightMap->GetTexture(), true);

	

	SceneNode* s = new SceneNode(heightMap, currentShader, heightMap->GetTexture());
	s->SetScale(Vector3(10, 10, 10));
	s->SetBoundingRadius(100000.0f);
	root->AddChild(s);


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	init = true;

}

Renderer6::~Renderer6()
{
	delete heightMap;
	delete root;
	delete camera;
	
}

void Renderer6::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);

	root->Update(msec);
}

void Renderer6::RenderScene()
{
	BuildNodeLists(root);
	SortNodeLists();

	//Set to a sky colour
	glClearColor(0.8f, 0.93f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	

	DrawNodes();

	SwapBuffers();
	ClearNodeLists();
}

void Renderer6::BuildNodeLists(SceneNode * from)
{

	//Build lists based on the transparency

	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
			transparentNodeList.push_back(from);
		else
			nodeList.push_back(from);


	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
		BuildNodeLists((*i));
}

void Renderer6::SortNodeLists()
{
	//Sort the lists based on distance from camera

	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer6::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer6::DrawNodes()
{
	//Draw each node

	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i)
		DrawNode((*i));

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i)
		DrawNode((*i));

}

void Renderer6::DrawNode(SceneNode * n)
{
	if (n->GetMesh() && n->GetShader()) {
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetScale());

		currentShader = n->GetShader();
		GLuint program = currentShader->GetProgram();

		glUseProgram(program);
		UpdateShaderMatrices();

		glUniform1i(glGetUniformLocation(program, "diffuseTex"), 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*)&transform);
/*
		glUniform4fv(glGetUniformLocation(program, "nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(program, "useTexture"), (int)n->GetMesh()->GetTexture());*/

		n->Draw(*this);

		glUseProgram(0);
	}

/*
	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
		DrawNode(*i);*/

}
