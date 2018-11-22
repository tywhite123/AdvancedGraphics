#include "Renderer13.h"





Renderer13::Renderer13(Window & parent) : OGLRenderer(parent)
{
	camera = new Camera();
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500.0f, RAW_HEIGHT*HEIGHTMAP_Z));
	light = new Light(Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500.0f, RAW_HEIGHT*HEIGHTMAP_Z / 2.0f),
		Vector4(1, 1, 1, 1), RAW_WIDTH*HEIGHTMAP_X , SPOT);

	reflectShader = new Shader(SHADERDIR"basicVertLight.glsl", SHADERDIR"ReflectFragment.glsl", "", SHADERDIR"WaterTessControl.glsl", SHADERDIR"WaterTessEval.glsl");
	skyboxShader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() || !skyboxShader->LinkProgram())
		return;


	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg",
		TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap())
		return;

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

}

Renderer13::~Renderer13()
{
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	currentShader = 0;
}

void Renderer13::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightmap();
	DrawWater();
	
	SwapBuffers();
}

void Renderer13::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 1000.0f;
	this->msec += msec * ((rand()%100)/25 + (rand()%20/5));
}

void Renderer13::DrawHeightmap()
{
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);

}

void Renderer13::DrawWater()
{

	quad->SetType(GL_PATCHES);
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"), msec);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "waterWave"), waterRotate);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH*HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 3.0f;
	float heightZ = (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);


}

void Renderer13::DrawSkybox()
{
	quad->SetType(GL_TRIANGLE_STRIP);
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	
	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}
