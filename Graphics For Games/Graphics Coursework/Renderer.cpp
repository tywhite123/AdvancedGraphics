#include "Renderer.h"



Renderer::Renderer(Window & parent) : OGLRenderer(parent)
{
	w = &parent;

	//TODO: Remove this
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	water = new HeightMap(TEXTUREDIR"terrain.raw", 0.0f);
	OBJMesh* m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"spyro2\\spyro.obj");
	spyro = m;
	quad = Mesh::GenerateQuad();
	particles = new ParticleBehaviour(1000, Vector3(0, 25000, 0), Vector4(1, 1, 1, 1), Vector3(0, 0, 0), 10000.0f);
	OBJMesh* objLight = new OBJMesh();
	objLight->LoadOBJMesh(MESHDIR"sphere.obj");
	Mesh* lightMesh = objLight;

	Mesh* geomPart = Mesh::GeneratePoints(100);
	geomPart->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	scene1 = new Scene1();
	scene2 = new Scene2();
	scene3 = new Scene3();

	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
	
	//camera = new Camera(-40,270, Vector3(-2000.0f, 3300.0f, 2000.0f));
	camera = new Camera(-40, 270, Vector3(10,15,10));
	root = new SceneNode();
	root2 = new SceneNode();
	root3 = new SceneNode();
	
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	//camera->SetPosition(Vector3(0, 30, 175));
	
	//TODO: Remove This
	terrainShader = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"BumpFragment.glsl");
	waterShader = new Shader(SHADERDIR"WaterVertex.glsl", SHADERDIR"WaterFragment.glsl");
	spyroShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	
	
	fontShader = new Shader(SHADERDIR"FontVert.glsl", SHADERDIR"FontFrag.glsl");
	skyboxShader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
	particleShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"colourFragment.glsl"/*, SHADERDIR"pointGeom.glsl"*/);
	lightShader = new Shader(SHADERDIR"matrixVertex.glsl", SHADERDIR"colourFragment.glsl");
	pointShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl", SHADERDIR"pointGeom.glsl");

	if (!terrainShader->LinkProgram() || !waterShader->LinkProgram() || !spyroShader->LinkProgram() || !fontShader->LinkProgram() || 
		!skyboxShader->LinkProgram() || !particleShader->LinkProgram() || !lightShader->LinkProgram() || !pointShader->LinkProgram()) {
		return;
	}
	cubeMap[0] = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg",
		TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	cubeMap[1] = SOIL_load_OGL_cubemap(TEXTUREDIR"city/darkcity_ft.tga", TEXTUREDIR"city/darkcity_bk.tga",
		TEXTUREDIR"city/darkcity_up.tga", TEXTUREDIR"city/darkcity_dn.tga", TEXTUREDIR"city/darkcity_rt.tga",
		TEXTUREDIR"city/darkcity_lf.tga", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	cubeMap[2] = SOIL_load_OGL_cubemap(TEXTUREDIR"plains/plains_ft.tga", TEXTUREDIR"plains/plains_bk.tga",
		TEXTUREDIR"plains/plains_up.tga", TEXTUREDIR"plains/plains_dn.tga", 
		TEXTUREDIR"plains/plains_rt.tga", TEXTUREDIR"plains/plains_lf.tga", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	grass = SOIL_load_OGL_texture(TEXTUREDIR"grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	

	if (/*!heightMap->GetTexture() || !water->GetTexture() || !heightMap->GetBumpMap() ||*/ !cubeMap || !grass)
		return;

	//TODO: Remove This
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(water->GetTexture(), true);


	SetTextureRepeating(grass, true);
	SetTextureRepeating(scene1->GetHeightMapTex(), true);
	SetTextureRepeating(scene1->GetHeightBumpMap(), true);
	SetTextureRepeating(scene1->GetWaterTex(), true);

	//light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X / 2.0f)*10, 5000.0f*10, (RAW_HEIGHT*HEIGHTMAP_Z) / 2.0f*10), Vector4(1, 1, 1, 1.0f), (RAW_WIDTH*HEIGHTMAP_X) / 2.0f * 60.0f, SPOT);
	light = new Light(Vector3(100.0f, 1000.0f, 100.0f), Vector4(1, 1, 1, 1), 5500.0f, SPOT);

	SceneNode* lightNode = new SceneNode(lightMesh, lightShader, Vector4(0.992f, 0.721f, 0.074f, 1.0f));
	lightNode->SetScale(Vector3((RAW_WIDTH*HEIGHTMAP_X) / 2.0f, (RAW_WIDTH*HEIGHTMAP_X) / 2.0f, (RAW_WIDTH*HEIGHTMAP_X) / 2.0f));
	lightNode->SetModelMatrix(Matrix4::Translation(light->GetPosition()));
	lightNode->SetBoundingRadius((RAW_WIDTH*HEIGHTMAP_X) / 2.0f * 15.0f);
	//root->AddChild(lightNode);

	SceneNode* pointNode = new SceneNode(geomPart, pointShader, Vector4(0.968f, 0.537f, 0.133f, 1));
	pointNode->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	pointNode->SetScale(Vector3(10, 10, 10));
	pointNode->SetBoundingRadius(100000.0f);
	//root->AddChild(pointNode);

	scene1->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	scene1->SetScale(Vector3(1, 1, 1));
	root->AddChild(scene1);

	scene2->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	scene2->SetScale(Vector3(1, 1, 1));
	root2->AddChild(scene2);

	scene3->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	scene3->SetScale(Vector3(1, 1, 1));
	root3->AddChild(scene3);


	particles->SetModelMatrix(Matrix4::Translation(Vector3(0, 0, 0)));
	particles->SetScale(Vector3(100, 100, 100));
	particles->SetBoundingRadius(100000.0f);
	particles->SetColour(Vector4(1.f, 1.f, 1.f, 0.9f));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	SetupShadows();

	time = 0;

	init = true;
	start = 0;
	end = 0;
	timeSinceProfile = 0;
	timeSinceSwitch = 0;
	framerate = 0;
	testEnd = 0;
	scene = 0;
	rain = false;
	profiler = false;
	switching = false;
	shadows = false;
	drawingShadows = false;
}

Renderer::~Renderer()
{
	delete heightMap;
	delete particles;
	delete water;
	delete spyro;
	delete root;
	delete root3;
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

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
	light->UpdateLight(msec);
	float test = w->GetTimer()->GetMS();
	if(scene == 0 && rain)
		particles->UpdateSystem(msec);
	testEnd = w->GetTimer()->GetMS() - test;
	

	time += msec;
	

	root->Update(msec);
	root2->Update(msec);
	root3->Update(msec);

	end = w->GetTimer()->GetMS();

	framerate++;
	float renderTime = end - start;
	rTime = "Render Time: " + to_string(renderTime);
	timeSinceProfile += renderTime;
	timeSinceSwitch += renderTime;
	if (timeSinceProfile >= 1000.0f) {
		fps = "FPS: " + to_string(framerate);
		
		timeSinceProfile = 0;
		framerate = 0;
	}

	start = w->GetTimer()->GetMS();
	if (w->GetKeyboard()->KeyDown(KEYBOARD_1))
		scene = 0;
	else if (w->GetKeyboard()->KeyDown(KEYBOARD_2))
		scene = 1;
	else if (w->GetKeyboard()->KeyDown(KEYBOARD_3))
		scene = 2;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_R))
		rain = !rain;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_F3))
		profiler = !profiler;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_F2)) {
		cout << projMatrix*viewMatrix << endl;
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {
		scene--;
		if (scene < 0) {
			scene = 2;
		}
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) {
		scene++;
		if (scene > 2) {
			scene = 0;
		}
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_PAUSE)) {
		switching = !switching;
	}


	if (switching && timeSinceSwitch >= 1000) {
		scene++;
		if (scene > 2) {
			scene = 0;
		}
		timeSinceSwitch = 0;
	}

}

void Renderer::RenderScene()
{
	if (scene == 0) {
		BuildNodeLists(root);
		shadows = false;
	}
	else if (scene == 1) {
		BuildNodeLists(root2);
		shadows = false;
	}
	else if (scene == 2) {
		BuildNodeLists(root3);
		shadows = true;
	}
	else {
		//TODO: Split Screen
	}
	SortNodeLists();

	//Draw the scene
	DrawScene();

	//If PostProcess Do DrawPostProcessEfffect 
	//Then Present The Scene

	//Maybe Add deffered

	//Draw the UI
	DrawUI();

	SwapBuffers();
	ClearNodeLists();
}

void Renderer::BuildNodeLists(SceneNode * from)
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

void Renderer::SortNodeLists()
{
	//Sort the lists based on distance from camera

	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawNodes()
{
	//Draw each node

	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i)
		DrawNode((*i));

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i)
		DrawNode((*i));

}

void Renderer::DrawNode(SceneNode * n)
{
	if (n->GetMesh() && n->GetShader()) {
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetScale());
		if (!drawingShadows) 
			currentShader = n->GetShader();
			
		GLuint program = currentShader->GetProgram();

		glUseProgram(program);
		UpdateShaderMatrices();
		if(!drawingShadows)
			SetShaderLight(*light);

		glUniform1i(glGetUniformLocation(program, "diffuseTex"), 0);
		glUniform1i(glGetUniformLocation(program, "bumpTex"), 1);
		glUniform1i(glGetUniformLocation(program, "cubeTex"), 2);
		glUniform1i(glGetUniformLocation(program, "grassTex"), 3);
		glUniform1i(glGetUniformLocation(program, "shadowTex"), 4);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap[scene]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, grass);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, shadowTex);

		Matrix4 shadowM = shadowMatrix * transform;

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*)&transform);
		glUniformMatrix4fv(glGetUniformLocation(program, "shadowMatrix"), 1, false, (float*)&shadowM);
		glUniform1f(glGetUniformLocation(program, "time"), time);
		glUniform1f(glGetUniformLocation(program, "particleSize"), n->GetScale().x*100);
		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*)&camera->GetPosition());
		glUniform4fv(glGetUniformLocation(program, "nodeColour"), 1, (float*)&n->GetColour());
/*
		glUniform1i(glGetUniformLocation(program, "useTexture"), (int)n->GetMesh()->GetTexture());*/

		n->Draw(*this);

		glUseProgram(0);
	}


}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	glCullFace(GL_FRONT);

	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap[scene]);
	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
}

void Renderer::DrawScene()
{
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	if (shadows) {
		DrawShadows();
		viewMatrix = camera->BuildViewMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
	}


	DrawNodes();

	float partStart = w->GetTimer()->GetMS();
	if (scene == 0 && rain) {
		DrawParticleSystem();
	}
	partEnd = (w->GetTimer()->GetMS() - start) + testEnd;

}

void Renderer::DrawParticleSystem()
{
	glDisable(GL_CULL_FACE);
	Matrix4 transform = particles->GetModelMatrix() * Matrix4::Translation(particles->GetCenter()) * Matrix4::Rotation(-90, Vector3(0, 0, 1)) * Matrix4::Scale(particles->GetScale());
	
	currentShader = particleShader;
	glUseProgram(currentShader->GetProgram());
	GLuint program = currentShader->GetProgram();

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*)&transform);
	glUniform4fv(glGetUniformLocation(program, "nodeColour"), 1, (float*)&particles->GetColour());
	particles->Draw(program);

	glUseProgram(0);
	glEnable(GL_CULL_FACE);

}

void Renderer::DrawShadows()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	shadowMatrix = biasMatrix*(projMatrix*viewMatrix);
	UpdateShaderMatrices();
	//glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, (float*)&shadowMatrix);
	drawingShadows = true;
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i)
		DrawNode((*i));
	drawingShadows = false;

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawPostProcess()
{
}

void Renderer::PresentScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//TODO: Set Shader HERE

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	viewMatrix.ToIdentity();

	UpdateShaderMatrices();

	//SetActiveTextures or Set quad texture

	quad->Draw();
	
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}

void Renderer::DrawUI()
{
	if (profiler) {
		DrawText(fps, Vector3(0, 0, 0), 16.0f);
		DrawText(rTime, Vector3(0, 20, 0), 16.0f);
		DrawText("Particle System: " + to_string(partEnd), Vector3(0, 40, 0), 16.0f);
		DrawText("Time Since Start: " + to_string(w->GetTimer()->GetMS() * 0.001), Vector3(0, 60, 0), 16.0f);

	}
	else {
		DrawText("Press F3 for the profiler", Vector3(0, 0, 0), 16.0f);
		DrawText("W: Forward, S: Backward, A:Left, D: Right", Vector3(0, 20, 0), 16.0f);
		DrawText("R: Toggle Particles", Vector3(0, 40, 0), 16.0f);
		DrawText("1: Scene 1, 2: Scene 2, 3: Scene 3, 4: Split Screen", Vector3(0, 60, 0), 16.0f);
		DrawText("Pause: Cycle Scenes", Vector3(0, 80, 0), 16.0f);
	}
	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SetupShadows()
{
	shadowShader = new Shader(SHADERDIR"ShadowVertex.glsl", SHADERDIR"ShadowFrag.glsl");
	if (!shadowShader->LinkProgram())
		return;

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	//GEN FRAME BUFFER
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GenerateScreenTextures(GLuint & into, bool depth)
{
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8, width, height, 0, depth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}




void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
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