#include "Renderer10.h"




Renderer10::Renderer10(Window & parent) : OGLRenderer(parent)
{
	camera = new Camera(0.0f, 135.0f, Vector3(0, 500, 0));
	camera2 = new Camera(0.0f, 50.0f, Vector3(0, 0, 600));
	camera3 = new Camera(0.0f, 135.0f, Vector3(600, 500, 600));
	camera4 = new Camera(0.0f, 135.0f, Vector3(300, 500, 0));
	quad = Mesh::GenerateQuad();

	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	sceneShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	processShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"processfrag.glsl");

	if (!heightMap->GetTexture())
		return;

	if (!sceneShader->LinkProgram())
		return;

	//if (!processShader->LinkProgram() /*|| !sceneShader->LinkProgram()*/ /*|| !heightMap->GetTexture()*/)
	//	return;

	if (!processShader->LinkProgram())
		return;

	SetTextureRepeating(heightMap->GetTexture(), true);

	//Generate the scene depth texture
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//COLOUR TEXTURE
	for (int i = 0; i < 8; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBO); //Render the scene into this
	glGenFramebuffers(1, &bufferFBO2); //Render the scene into this
	glGenFramebuffers(1, &processFBO); //Post process in this
	glGenFramebuffers(1, &bufferFBO3); //Render the scene into this
	glGenFramebuffers(1, &bufferFBO4); //Render the scene into this

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

	//Check FBO attachment success
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0])
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO2);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[2], 0);

	//Check FBO attachment success
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[2])
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO3);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[4], 0);

	//Check FBO attachment success
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[4])
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO4);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[6], 0);

	//Check FBO attachment success
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[6])
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	init = true;

}

Renderer10::~Renderer10()
{
	delete sceneShader;
	delete processShader;
	currentShader = NULL;

	delete heightMap;
	delete quad;
	delete camera;
	delete camera2;
	delete camera3;
	delete camera4;

	glDeleteTextures(4, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
	glDeleteFramebuffers(1, &bufferFBO2);
	glDeleteFramebuffers(1, &bufferFBO3);
	glDeleteFramebuffers(1, &bufferFBO4);
}

void Renderer10::RenderScene()
{
	DrawScene();
	DrawPostProcess();
	PresentScene();
	SwapBuffers();
}

void Renderer10::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	camera2->UpdateCamera(msec);
	camera3->UpdateCamera(msec);
	camera4->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer10::PresentScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(sceneShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	//TOP LEFT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.5f,-0.5f,0))* Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(bufferColourTex[0]);
	quad->Draw();

	//BOTTOM LEFT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.5f, 0.5f, 0))*Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(bufferColourTex[2]);
	quad->Draw();

	//TOP RIGHT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.5f, -0.5f, 0))* Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(bufferColourTex[4]);
	quad->Draw();

	//BOTTOM RIGHT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.5f, 0.5f, 0))*Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(bufferColourTex[6]);
	quad->Draw();
	glUseProgram(0);
}

void Renderer10::DrawPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(processShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);


	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);


	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

		quad->SetTexture(bufferColourTex[0]);
		quad->Draw();

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

		quad->SetTexture(bufferColourTex[1]);
		quad->Draw();

	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);

}


//Renders the heightmap scene into the FBO buffer
void Renderer10::DrawScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetCurrentShader(sceneShader);

	//Set perspective of the scene
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	viewMatrix = camera2->BuildViewMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO2);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetCurrentShader(sceneShader);

	//Set perspective of the scene
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices();

	heightMap->Draw();


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	viewMatrix = camera3->BuildViewMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO3);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetCurrentShader(sceneShader);

	//Set perspective of the scene
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices();

	heightMap->Draw();


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	viewMatrix = camera4->BuildViewMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO4);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetCurrentShader(sceneShader);

	//Set perspective of the scene
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 150.0f);

	UpdateShaderMatrices();

	heightMap->Draw();


	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}



