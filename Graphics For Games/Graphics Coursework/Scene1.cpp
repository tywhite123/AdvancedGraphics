#include "Scene1.h"


#include "..\..\nclgl\OBJMesh.h"

Scene1::Scene1()
{
	//heightMap = new HeightMap(TEXTUREDIR"ilkley.raw");
	hmap = new HeightMapFromJPG(TEXTUREDIR"ilkley.jpg");
	water = new HeightMap(TEXTUREDIR"terrain.raw", 0.0f);
	OBJMesh* m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"spyro2\\spyro.obj");
	spyro = m;
	quad = Mesh::GenerateQuad();
	/*particles = new ParticleBehaviour(1000, Vector3(0, 0, 0), Vector4(1, 1, 1, 1), Vector3(0, 0, 0), 500.0f);*/


	//camera = new Camera(-40, 270, Vector3(-2000.0f, 3300.0f, 2000.0f));

	//basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	//camera->SetPosition(Vector3(0, 30, 175));


	terrainShader = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"TerrainFragment.glsl");
	waterShader = new Shader(SHADERDIR"WaterVertex.glsl", SHADERDIR"WaterFragment.glsl");
	spyroShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	//fontShader = new Shader(SHADERDIR"FontVert.glsl", SHADERDIR"FontFrag.glsl");
	skyboxShader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
//	particleShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"colourFragment.glsl");

	if (!terrainShader->LinkProgram() || !waterShader->LinkProgram() || !spyroShader->LinkProgram() || /*!fontShader->LinkProgram() ||*/
		!skyboxShader->LinkProgram() /*|| !particleShader->LinkProgram()*/) {
		return;
	}

	hmap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	hmap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	water->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA"/*"Barren Reds.JPG"*/, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg",
		TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);


	if (!hmap->GetTexture() || !water->GetTexture() || !hmap->GetBumpMap()|| !cubeMap)
		return;

	//SET TEXTURE REPEATING

	/*light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X / 2.0f) * 10, 100.0f * 10, (RAW_HEIGHT*HEIGHTMAP_Z) / 2.0f * 10), Vector4(1, 1, 1, 0.7f), (RAW_WIDTH*HEIGHTMAP_X) / 2.0f * 15.0f);*/

	SceneNode* terrainNode = new SceneNode(hmap, terrainShader,hmap->GetTexture());
	terrainNode->SetScale(Vector3(10, 10, 10));
	terrainNode->SetBoundingRadius(100000.0f);
	AddChild(terrainNode);

	SceneNode* waterNode = new SceneNode(water, waterShader, water->GetTexture());
	waterNode->SetScale(Vector3(20, 20, 20));
	waterNode->SetColour(Vector4(1, 1, 1, 0.5f));
	waterNode->SetModelMatrix(Matrix4::Translation(Vector3(0, 6000, 0)));
	waterNode->SetBoundingRadius(100000.0f);
	AddChild(waterNode);

	SceneNode* spyroNode = new SceneNode(spyro, spyroShader, spyro->GetTexture());
	spyroNode->SetScale(Vector3(1000, 1000, 1000));
	spyroNode->SetModelMatrix(Matrix4::Translation(Vector3(10000, 3000, 15000)) * Matrix4::Rotation(90.0f, Vector3(1, 1, 0)));
	spyroNode->SetBoundingRadius(1000.0f);
	AddChild(spyroNode);


	//particles->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	//particles->SetScale(Vector3(50, 50, 50));
	//particles->SetBoundingRadius(100000.0f);
	//particles->SetColour(Vector4(0.3f, 0.3f, 1, 0.9f));
}


Scene1::~Scene1()
{
}
