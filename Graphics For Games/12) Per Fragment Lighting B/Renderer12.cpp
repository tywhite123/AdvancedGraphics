#include "Renderer12.h"


Renderer12::Renderer12(Window & parent) : OGLRenderer(parent)
{
	camera = new Camera(0.0f, 0.0f, Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500.0f, RAW_HEIGHT*HEIGHTMAP_Z));

	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	currentShader = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"BumpFragment.glsl");

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!currentShader->LinkProgram() || !heightMap->GetTexture() || !heightMap->GetBumpMap())
		return;


	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X / 2.0f), 1000.0f, (RAW_HEIGHT*HEIGHTMAP_Z) / 5.0f), Vector4(1, 1, 1, 1), (RAW_WIDTH*HEIGHTMAP_X)/ 2.0f, SPOT);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	init = true;

}

Renderer12::~Renderer12()
{
	delete camera;
	delete heightMap;
	delete light;
}

void Renderer12::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());


	UpdateShaderMatrices();
	SetShaderLight(*light);
	heightMap->Draw();

	glUseProgram(0);
	SwapBuffers();
}

void Renderer12::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}
