#include "Renderer5.h"



Renderer6::Renderer6(Window & parent) : OGLRenderer(parent)
{
	CubeRobot::CreateCube();
	camera = new Camera();

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 30, 175));
	
	
	shader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");

	if (!shader->LinkProgram()) {
		return;
	}

	quad = Mesh::GenerateQuad();
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!quad->GetTexture())
		return;

	root = new SceneNode();

	

	for (int i = 0; i < 5; ++i) {
		SceneNode* s = new SceneNode(quad, shader, Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetModelMatrix(Matrix4::Translation(Vector3(0.0f, 100.0f, -300.0f + 100.0f + 100 * i)));
		s->SetScale(Vector3(100.0f, 100.0f, 100.0f));
		s->SetBoundingRadius(100.0f);
		root->AddChild(s);
	}

	root->AddChild(new CubeRobot());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	init = true;

}

Renderer6::~Renderer6()
{
	delete root;
	delete quad;
	delete camera;
	CubeRobot::DeleteCube();
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

	for (vector<SceneNode*>::const_iterator i = transparentNodeList.begin(); i != transparentNodeList.end(); ++i)
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

		glUniform1i(glGetUniformLocation(program, "diffuseTex"), 1);

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*)&transform);

		glUniform4fv(glGetUniformLocation(program, "nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(program, "useTexture"), (int)n->GetMesh()->GetTexture());

		n->Draw(*this);

		glUseProgram(0);
	}

	/*for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
		DrawNode(*i);*/

}