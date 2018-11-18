#include "Renderer9.h"
#include "..\..\nclgl\MD5Node.h"



Renderer9::Renderer9(Window & parent) : OGLRenderer(parent)
{
	camera = new Camera(0, 90.0f, Vector3(-180, 60, 0));
	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	if (!currentShader->LinkProgram())
		return;


	hellData->AddAnim(MESHDIR"walk7.md5anim");
	hellNode->PlayAnim(MESHDIR"walk7.md5anim");


	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	init = true;

}

Renderer9::~Renderer9()
{
	delete camera;
	delete hellData;
	delete hellNode;
}

void Renderer9::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	UpdateShaderMatrices();

	hellNode->Draw(*this);


	glUseProgram(0);
	SwapBuffers();

}

void Renderer9::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();

	hellNode->Update(msec);
	//TODO: sort this out tomorrow
	if (hellNode->GetCurrentFrame() == hellNode->GetNumFrames()) {
		hellNode->SetModelMatrix(hellNode->GetModelMatrix() * hellNode->GetSkeleton()->joints[0].transform);
	}
}
