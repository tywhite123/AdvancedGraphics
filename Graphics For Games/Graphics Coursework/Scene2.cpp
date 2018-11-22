#include "Scene2.h"
#include "..\..\nclgl\OBJMesh.h"


Scene2::Scene2()
{
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	OBJMesh* m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"tundratri.obj");
	winterTundra = m;
	quad = Mesh::GenerateQuad();
	
	terrainShader = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"TerrainFragment.glsl");
	spyroShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	waterShader = new Shader(SHADERDIR"basicVertLight.glsl", SHADERDIR"ReflectFragment.glsl", "", SHADERDIR"WaterTessControl.glsl", SHADERDIR"WaterTessEval.glsl");
	skyboxShader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
	

	if (!spyroShader->LinkProgram() || !skyboxShader->LinkProgram() || !waterShader->LinkProgram() || !terrainShader->LinkProgram()) {
		return;
	}

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));


	if (!quad->GetTexture())
		return;

	

	/*SceneNode* spyroNode = new SceneNode(spyro, spyroShader, spyro->GetTexture());
	spyroNode->SetScale(Vector3(1000, 1000, 1000));
	spyroNode->SetModelMatrix(Matrix4::Translation(Vector3(10000, 3000, 15000)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	spyroNode->SetBoundingRadius(1000.0f);
	AddChild(spyroNode);*/



	SceneNode* tundraNode = new SceneNode(winterTundra, spyroShader, winterTundra->GetTexture());
	tundraNode->SetScale(Vector3(1, 1, 1));
	tundraNode->SetModelMatrix(Matrix4::Translation(Vector3(0, -1000.0f, 0)) * Matrix4::Rotation(-90.0f, Vector3(1,0,0)));
	tundraNode->SetBoundingRadius(1000000.0f);
	AddChild(tundraNode);


	float heightX = (RAW_WIDTH*HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 3.0f;
	float heightZ = (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f);
	quad->SetType(GL_PATCHES);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	SceneNode* waterNode = new SceneNode(quad, waterShader, quad->GetTexture());
	waterNode->SetScale(Vector3	(heightX, heightZ, 1));
	waterNode->SetModelMatrix(Matrix4::Translation(Vector3(-200.0f, 0, 0)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	waterNode->SetBoundingRadius(1000000.0f);
	AddChild(waterNode);
	SceneNode* waterNode2 = new SceneNode(quad, waterShader, quad->GetTexture());
	waterNode2->SetScale(Vector3(heightX, heightZ, 1));
	waterNode2->SetModelMatrix(Matrix4::Translation(Vector3(8000.0f, 0, 0)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	waterNode2->SetBoundingRadius(1000000.0f);
	AddChild(waterNode2);
	SceneNode* waterNode3 = new SceneNode(quad, waterShader, quad->GetTexture());
	waterNode3->SetScale(Vector3(heightX, heightZ, 1));
	waterNode3->SetModelMatrix(Matrix4::Translation(Vector3(-200.0f, 0, -8200.0f)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	waterNode3->SetBoundingRadius(1000000.0f);
	AddChild(waterNode3);
	SceneNode* waterNode4 = new SceneNode(quad, waterShader, quad->GetTexture());
	waterNode4->SetScale(Vector3(heightX, heightZ, 1));
	waterNode4->SetModelMatrix(Matrix4::Translation(Vector3(8000.0f, 0, -8200.0f)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	waterNode4->SetBoundingRadius(1000000.0f);
	AddChild(waterNode4);
	SceneNode* terrainNode = new SceneNode(heightMap, terrainShader, heightMap->GetTexture());
	terrainNode->SetScale(Vector3(4,3,4));
	terrainNode->SetModelMatrix(Matrix4::Translation(Vector3(-4000.0f,-500.0f,-13000.0f)));
	terrainNode->SetBoundingRadius(1000000.0f);
	AddChild(terrainNode);


}


Scene2::~Scene2()
{
	delete winterTundra;
	delete heightMap;
	delete quad;
	delete terrainShader;
	delete waterShader;
	delete spyroShader;
	delete skyboxShader;
}
