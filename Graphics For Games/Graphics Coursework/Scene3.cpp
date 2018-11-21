#include "Scene3.h"

#include "..\..\nclgl\OBJMesh.h"


Scene3::Scene3()
{

	//hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	//hellNode = new MD5Node(*hellData);

	//hellData->AddAnim(MESHDIR"idle2.md5anim");
	//hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	OBJMesh* obj = new OBJMesh();
	obj->LoadOBJMesh(MESHDIR"spyro2\\spyro.obj");
	spyro = obj;

	sceneShader = new Shader(SHADERDIR"ShadowSceneVertex.glsl", SHADERDIR"ShadowSceneFrag.glsl");
	hellKnightSceneShader = new Shader(SHADERDIR"skeletonVertex.glsl", SHADERDIR"ShadowSceneFrag.glsl");
	
	//sceneShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	//shadowShader = new Shader(SHADERDIR"ShadowVertex.glsl", SHADERDIR"ShadowFrag.glsl");

	if (!sceneShader->LinkProgram() || !hellKnightSceneShader->LinkProgram()/* || !shadowShader->LinkProgram()*/)
		return;

	//SetUpFBOs();

	floor = Mesh::GenerateQuad();
	floor->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	floor->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"brickDOT3.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	SceneNode* floorNode = new SceneNode(floor, sceneShader, floor->GetTexture());
	floorNode->SetColour(Vector4(1, 1, 1, 1));
	floorNode->SetScale(Vector3(1000, 1000, 1));
	floorNode->SetModelMatrix(Matrix4::Translation(Vector3(0,0,0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	floorNode->SetBoundingRadius(10000.0f);
	AddChild(floorNode);

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	//
	hellData->AddAnim(MESHDIR"walk7.md5anim");
	int x;
	for (int i = 0; i < 25; ++i) {
		if(i%5 == 0)
			x = i*50.0f;
		hellNode = new MD5Node(*hellData);
		hellNode->PlayAnim(MESHDIR"walk7.md5anim");
		hellNode->SetShader(hellKnightSceneShader);
		hellNode->SetScale(Vector3(2, 2, 2));
		if(i%200)
		hellNode->SetModelMatrix(Matrix4::Translation(Vector3(x, 0, ((i%5)*200.0f))));
		hellNode->SetBoundingRadius(1000.0f);
		hellNode->SetSkeletal(true);
		AddChild(hellNode);
	}

	SceneNode* spyroNode = new SceneNode(spyro, sceneShader, Vector4(1, 1, 1, 1));
	spyroNode->SetScale(Vector3(100, 100, 100));
	spyroNode->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -300)));
	spyroNode->SetBoundingRadius(1000.0f);
	//AddChild(spyroNode);

}


Scene3::~Scene3()
{
}

void Scene3::Update(float msec)
{
	SceneNode::Update(msec);
	//hellNode->Update(msec);
	
}
