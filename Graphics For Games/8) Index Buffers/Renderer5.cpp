#include "Renderer5.h"



Renderer6::Renderer6(Window & parent) : OGLRenderer(parent)
{
	w = &parent;
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	water = new HeightMap(TEXTUREDIR"terrain.raw", 0.0f);
	OBJMesh* m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"spyro2\\spyro.obj");
	spyro = m;
	quad = Mesh::GenerateQuad();
	particles = new ParticleBehaviour(1000, Vector3(0, 0, 0), Vector4(1, 1, 1, 1), Vector3(0, 0, 0), 5000.0f);

	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
	
	camera = new Camera(-40,270, Vector3(-2000.0f, 3300.0f, 2000.0f));
	root = new SceneNode();
	
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	//camera->SetPosition(Vector3(0, 30, 175));
	
	
	terrainShader = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"BumpFragment.glsl");
	waterShader = new Shader(SHADERDIR"WaterVertex.glsl", SHADERDIR"WaterFragment.glsl");
	spyroShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	fontShader = new Shader(SHADERDIR"FontVert.glsl", SHADERDIR"FontFrag.glsl");
	skyboxShader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
	particleShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"colourFragment.glsl");

	if (!terrainShader->LinkProgram() || !waterShader->LinkProgram() || !spyroShader->LinkProgram() || !fontShader->LinkProgram() || 
		!skyboxShader->LinkProgram() || !particleShader->LinkProgram()) {
		return;
	}

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	water->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.TGA"/*"Barren Reds.JPG"*/, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg",
		TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	

	if (!heightMap->GetTexture() || !water->GetTexture() || !heightMap->GetBumpMap() || !cubeMap)
		return;

	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(water->GetTexture(), true);

	light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X / 2.0f)*10, 1000.0f*10, (RAW_HEIGHT*HEIGHTMAP_Z) / 2.0f*10), Vector4(1, 1, 1, 1), (RAW_WIDTH*HEIGHTMAP_X) / 2.0f * 15.0f);

	SceneNode* terrainNode = new SceneNode(heightMap, terrainShader, heightMap->GetTexture());
	terrainNode->SetScale(Vector3(10, 10, 10));
	terrainNode->SetBoundingRadius(100000.0f);
	root->AddChild(terrainNode);

	SceneNode* waterNode = new SceneNode(water, waterShader, water->GetTexture());
	waterNode->SetScale(Vector3(10, 10, 10));
	waterNode->SetColour(Vector4(1, 1, 1, 0.5f));
	waterNode->SetModelMatrix(Matrix4::Translation(Vector3(0, 2000, 0)));
	waterNode->SetBoundingRadius(100000.0f);
	root->AddChild(waterNode);

	SceneNode* spyroNode = new SceneNode(spyro, spyroShader, spyro->GetTexture());
	spyroNode->SetScale(Vector3(1000, 1000, 1000));
	spyroNode->SetModelMatrix(Matrix4::Translation(Vector3(10000, 3000, 15000)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	spyroNode->SetBoundingRadius(100000.0f);
	root->AddChild(spyroNode);


	particles->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	particles->SetScale(Vector3(10, 10, 10));
	particles->SetBoundingRadius(100000.0f);
	particles->SetColour(Vector4(1, 1, 1, 1));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	time = 0;

	init = true;
	start = 0;
	end = 0;
	timeSinceProfile = 0;
	framerate = 0;
}

Renderer6::~Renderer6()
{
	delete heightMap;
	delete water;
	delete spyro;
	delete root;
	delete camera;
	delete quad;
	delete waterShader;
	delete terrainShader;
	delete spyroShader;
	delete skyboxShader;
	delete basicFont;
	delete fontShader;
	delete particleShader;
	delete light;
	currentShader = NULL;
	
}

void Renderer6::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
	light->UpdateLight(msec);
	particles->UpdateSystem(msec);

	time += msec;

	root->Update(msec);

	end = w->GetTimer()->GetMS();

	framerate++;
	float renderTime = end - start;
	rTime = "Render Time: " + to_string(renderTime);
	timeSinceProfile += renderTime;
	if (timeSinceProfile >= 1000.0f) {
		fps = "FPS: " + to_string(framerate);
		
		timeSinceProfile = 0;
		framerate = 0;
	}

	start = w->GetTimer()->GetMS();

}

void Renderer6::RenderScene()
{
	BuildNodeLists(root);
	SortNodeLists();

	//Set to a sky colour
	//glClearColor(0.8f, 0.93f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	
	DrawSkybox();
	DrawNodes();
	DrawParticleSystem();
	DrawText(fps, Vector3(0, 0, 0), 16.0f);
	DrawText(rTime, Vector3(0, 20, 0), 16.0f);
	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);

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
		SetShaderLight(*light);

		glUniform1i(glGetUniformLocation(program, "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(program, "bumpTex"), 1);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*)&transform);
		glUniform1f(glGetUniformLocation(program, "time"), time);
		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		glUniform4fv(glGetUniformLocation(program, "nodeColour"), 1, (float*)&n->GetColour());
/*
		glUniform1i(glGetUniformLocation(program, "useTexture"), (int)n->GetMesh()->GetTexture());*/

		n->Draw(*this);

		glUseProgram(0);
	}

/*
	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
		DrawNode(*i);*/

}

void Renderer6::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	glCullFace(GL_FRONT);

	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
}

void Renderer6::DrawParticleSystem()
{
	glDisable(GL_CULL_FACE);
	Matrix4 transform = particles->GetWorldTransform() * Matrix4::Translation(particles->GetCenter()) * Matrix4::Scale(particles->GetScale());


	
	currentShader = particleShader;
	glUseProgram(currentShader->GetProgram());
	GLuint program = currentShader->GetProgram();

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*)&transform);
	glUniform4fv(glGetUniformLocation(program, "nodeColour"), 1, (float*)&particles->GetColour());

	particles->Draw(glGetUniformLocation(program, "particleMatrix"));

	glUseProgram(0);
	glEnable(GL_CULL_FACE);

}



void Renderer6::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	currentShader = fontShader;
	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	UpdateShaderMatrices();
	mesh->Draw();
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	delete mesh; //Once it's drawn, we don't need it anymore!
}