#include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent)
{

	triangle = Mesh::GenerateTriangle();

	

	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}
	
	triangle->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!triangle->GetTexture()) {
		return;
	}

	init = true;

	//SwitchToOrthographic();

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, 1, -1);


	filtering = true;
	repeating = false;
}

Renderer::~Renderer()
{
	delete triangle;
}

void Renderer::RenderScene()
{
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());

	/*glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);*/
/*
	for (int i = 0; i < 3; ++i) {
		Vector3 tempPos = position;
		tempPos.z += (i*500.0f);
		tempPos.x -= (i*100.0f);
		tempPos.y -= (i*100.0f);

		modelMatrix = Matrix4::Translation(tempPos) *
			Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
			Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);*/
		
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	triangle->Draw();

	/*}*/


	glUseProgram(0);

	SwapBuffers();

}

void Renderer::SwitchToPerspective()
{
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic()
{
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::UpdateTextureMatrix(float rot)
{
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(rot, Vector3(0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;
}

void Renderer::ToggleRepeating()
{
	repeating = !repeating;
	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP); //X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP); //Y
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::ToggleFiltering()
{
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, triangle->GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST); 
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}