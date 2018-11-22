#include "Renderer.h"



Renderer::Renderer(Window & parent) : OGLRenderer(parent)
{
	w = &parent;

	//TODO: Remove this
	quad = Mesh::GenerateQuad();
	particles = new ParticleBehaviour(1000, Vector3(0, 25000, 0), Vector4(1, 1, 1, 1), Vector3(0, 0, 0), 10000.0f);



	Mesh* geomPart = Mesh::GeneratePoints(100);
	geomPart->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	scene1 = new Scene1();
	scene2 = new Scene2();
	scene3 = new Scene3();

	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
	
	camera = new Camera(0,230, Vector3(-2000.0f, 10000.0f, 2000.0f));
	camera2 = new Camera(0, 320, Vector3(-3000,1000,3000));
	camera3 = new Camera(-40, 210, Vector3(-1000,2000,-2000));
	//camera = new Camera(-40, 270, Vector3(10,15,10));
	//camera = new Camera(-40, 270, Vector3(0, 100, 0));
	root = new SceneNode();
	root2 = new SceneNode();
	root3 = new SceneNode();
	
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);


	
	
	fontShader = new Shader(SHADERDIR"FontVert.glsl", SHADERDIR"FontFrag.glsl");
	skyboxShader = new Shader(SHADERDIR"SkyboxVertex.glsl", SHADERDIR"SkyboxFragment.glsl");
	particleShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"colourFragment.glsl"/*, SHADERDIR"pointGeom.glsl"*/);
	lightShader = new Shader(SHADERDIR"matrixVertex.glsl", SHADERDIR"colourFragment.glsl");
	pointShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl", SHADERDIR"pointGeom.glsl");
	screenShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"ScreenFrag.glsl");
	processShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"processfrag.glsl");
	splitScreenShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!fontShader->LinkProgram() || !skyboxShader->LinkProgram() || !particleShader->LinkProgram() || !lightShader->LinkProgram() || !pointShader->LinkProgram()
		|| !screenShader->LinkProgram() || !processShader->LinkProgram() || !splitScreenShader->LinkProgram()) {
		return;
	}
	cubeMap[0] = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg",
		TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	cubeMap[1] = SOIL_load_OGL_cubemap(TEXTUREDIR"mystic/mystic_ft.tga", TEXTUREDIR"mystic/mystic_bk.tga",
		TEXTUREDIR"mystic/mystic_up.tga", TEXTUREDIR"mystic/mystic_dn.tga", TEXTUREDIR"mystic/mystic_rt.tga",
		TEXTUREDIR"mystic/mystic_lf.tga", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	cubeMap[2] = SOIL_load_OGL_cubemap(TEXTUREDIR"city/darkcity_ft.tga", TEXTUREDIR"city/darkcity_bk.tga",
		TEXTUREDIR"city/darkcity_up.tga", TEXTUREDIR"city/darkcity_dn.tga", TEXTUREDIR"city/darkcity_rt.tga",
		TEXTUREDIR"city/darkcity_lf.tga", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	grass = SOIL_load_OGL_texture(TEXTUREDIR"grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	

	if (!cubeMap || !grass)
		return;


	SetTextureRepeating(grass, true);
	SetTextureRepeating(scene1->GetHeightMapTex(), true);
	SetTextureRepeating(scene1->GetHeightBumpMap(), true);
	SetTextureRepeating(scene1->GetWaterTex(), true);
	SetTextureRepeating(scene2->GetHeightMapTex(), true);
	SetTextureRepeating(scene2->GetHeightBumpMap(), true);

	light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X / 2.0f)*10, 5000.0f*10, (RAW_HEIGHT*HEIGHTMAP_Z) / 2.0f*10), Vector4(1, 1, 1, 1.0f), (RAW_WIDTH*HEIGHTMAP_X) / 2.0f * 60.0f,DIRECTIONAL);
	light3 = new Light(Vector3(2000.0f, 6000.0f, 100.0f), Vector4(1, 1, 1, 1), 1200000.0f, SPOT);

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
	SetupScreen();
	SetupPostProcess();
	SetupSplitScreen();

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
	post = false;
	pType = SOBEL;
	splitScreen = false;
	snowVal = -1;

	camera->SetPanningDir(Vector3(2, 0, 1));
	camera2->SetPanningDir(Vector3(0.5, 0.25, -0.5));
	camera3->SetPanningDir(Vector3(0,0,0));
}

Renderer::~Renderer()
{
	delete root;
	delete root2;
	delete root3;
	delete particles;
	delete camera;
	delete camera2;
	delete camera3;
	delete quad;
	delete light;
	delete light3;
	
	delete basicFont;


	delete fontShader;
	delete particleShader;
	delete skyboxShader;
	delete screenShader;
	delete processShader;
	delete shadowShader;
	delete pointShader;
	delete splitScreenShader;

	glDeleteTextures(1, &shadowTex);
	glDeleteTextures(3, sceneColourTex);
	glDeleteTextures(1, &sceneDepthTex);
	glDeleteTextures(3, splitColourTex);
	glDeleteTextures(3, splitDepthTex);

	glDeleteFramebuffers(1, &shadowFBO);
	glDeleteFramebuffers(1, &sceneFBO);
	glDeleteFramebuffers(1, &processFBO);
	glDeleteFramebuffers(3, splitScreenFBO);


	currentShader = NULL;
	
}

void Renderer::UpdateScene(float msec)
{
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
	light->UpdateLight(msec);
	float test = w->GetTimer()->GetMS();
	if (scene == 0 && rain) {
		particles->UpdateSystem(msec);
		snowVal += 0.00015f * msec;
		if (snowVal > 2.0f) {
			snowVal = 2;
		}
	}
	else {
		snowVal -= 0.00015f * msec;
		if (snowVal < -1.0f) {
			snowVal = -1;
		}
	}
	testEnd = w->GetTimer()->GetMS() - test;
	waterRotate += msec / 1000.0f;
	waterTime += msec * ((rand() % 100) / 25 + (rand() % 20 / 5));

	time += msec;
	
	if (splitScreen) {
		root->Update(msec);
		root2->Update(msec);
		root3->Update(msec);
		camera->UpdateCamera(msec);
		camera2->UpdateCamera(msec);
		camera3->UpdateCamera(msec);
	}
	else if (scene == 0) {
		camera->UpdateCamera(msec, switching);
		root->Update(msec);
	}
	else if (scene == 1) {
		camera2->UpdateCamera(msec, switching);
		root2->Update(msec);
	}
	else if (scene == 2) {
		root3->Update(msec);
		light3->UpdateLight(msec);
		camera3->UpdateCamera(msec, switching);
	}

	end = w->GetTimer()->GetMS();

	framerate++;
	float renderTime = end - start;
	rTime = "Render Time: " + to_string(renderTime);
	timeSinceProfile += renderTime;
	if(switching)
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
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_4))
		splitScreen = !splitScreen;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_R))
		rain = !rain;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_F3))
		profiler = !profiler;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_F2)) {
		cout << projMatrix*viewMatrix << endl;
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {
		scene--;
		ResetCameras();
		if (scene < 0) {
			scene = 2;
		}
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) {
		scene++;
		ResetCameras();
		if (scene > 2) {
			scene = 0;
		}
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_PAUSE)) {
		switching = !switching;
		ResetCameras();
	}
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_P))
		post = !post;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_NUMPAD1))
		pType = BLUR;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_NUMPAD2))
		pType = SOBEL;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_NUMPAD3))
		pType = BLOOM;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_NUMPAD4))
		pType = INVERT;
	else if (w->GetKeyboard()->KeyTriggered(KEYBOARD_NUMPAD5))
		pType = GRAYSCALE;

	if (switching && timeSinceSwitch >= 8000) {
		scene++;
		if (scene > 2) {
			scene = 0;
			ResetCameras();
		}
		timeSinceSwitch = 0;
	}

}

void Renderer::RenderScene()
{
	Vector3 originalCameraPos = camera->GetPosition();
	if (scene == 0) {
		viewMatrix = camera->BuildViewMatrix();
		BuildNodeLists(root);
		shadows = false;
	}
	else if (scene == 1) {
		BuildNodeLists(root2);
		viewMatrix = camera2->BuildViewMatrix();
		shadows = false;
	}
	else if (scene == 2) {
		viewMatrix = camera3->BuildViewMatrix();
		BuildNodeLists(root3);
		shadows = true;
	}
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//Draw the scene
	if (splitScreen) {
		
		int originalScene = scene;
		//Build Scene 1
		scene = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, splitScreenFBO[0]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		ClearNodeLists();
		BuildNodeLists(root);
		SortNodeLists();
		DrawSkybox();
		DrawNodes();
		if (scene == 0 && rain) {
			DrawParticleSystem();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		//viewMatrix = viewMatrix * Matrix4::Translation(Vector3(20000, 6000, 10000));
		scene = 1;
		//Build Scene2
		viewMatrix = camera2->BuildViewMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, splitScreenFBO[1]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		ClearNodeLists();
		BuildNodeLists(root2);
		SortNodeLists();
		DrawSkybox();
		DrawNodes();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		scene = 2;
		//Build Sceen3
		if (shadows) {
			DrawShadows();
			viewMatrix = camera->BuildViewMatrix();
			glClear(GL_DEPTH_BUFFER_BIT);
			DrawSkybox();
		}
		viewMatrix = camera3->BuildViewMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, splitScreenFBO[2]);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		ClearNodeLists();
		BuildNodeLists(root3);
		SortNodeLists();
		DrawSkybox();
		DrawNodes();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		scene = originalScene;
		DrawSplitScreen();
	}
	else {
		DrawScene();

		//If PostProcess Do DrawPostProcessEfffect 
		if (post)
			DrawPostProcess();
		//Then Present The Scene
		PresentScene();
	}

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
		if (scene == 0 || scene == 1) {
			SetShaderLight(*light);
		}
		else {
			if (!drawingShadows)
				SetShaderLight(*light3);
		}

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
		glUniform1f(glGetUniformLocation(program, "waterTime"), time);
		glUniform1f(glGetUniformLocation(program, "snow"), snowVal);
		glUniform1f(glGetUniformLocation(program, "waterWave"), waterRotate);
		glUniform1i(glGetUniformLocation(program, "skeletal"), (int)n->GetSkeletal());
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
	if (shadows) {
		DrawShadows();
		viewMatrix = camera3->BuildViewMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
		//DrawSkybox();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();

	DrawNodes();

	float partStart = w->GetTimer()->GetMS();
	if (scene == 0 && rain) {
		DrawParticleSystem();
	}
	partEnd = (w->GetTimer()->GetMS() - start) + testEnd;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	viewMatrix = Matrix4::BuildViewMatrix(light3->GetPosition(), Vector3(0, 0, 0));
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
	PostType currentType = pType;
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[2], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//SetShader
	SetCurrentShader(processShader);

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (pType == BLOOM) {
		glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "effectType"), BLOOM);
		quad->SetTexture(sceneColourTex[0]);
		quad->Draw();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[2], 0);
		pType = BLUR;
	}

	if (pType == SOBEL) {
		glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "effectType"), SOBEL);
		quad->SetTexture(sceneColourTex[0]);
		quad->Draw();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[1], 0);
		quad->SetTexture(sceneColourTex[1]);
		quad->Draw();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[0], 0);
		//pType = BLUR;
	}

	if (pType == BLUR) {
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "effectType"), BLUR);
		for (int i = 0; i < POST_PASSES; ++i) {

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[1], 0);
			if(currentType == pType)
				quad->SetTexture(sceneColourTex[0]);
			else
				quad->SetTexture(sceneColourTex[2]);
			quad->Draw();

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
			
			if (currentType == pType)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[0], 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[2], 0);

			quad->SetTexture(sceneColourTex[1]);
			quad->Draw();


		}
	}

	if (pType == INVERT) {
		glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "effectType"), INVERT);
		quad->SetTexture(sceneColourTex[0]);
		quad->Draw();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[0], 0);
	}
	if (pType == GRAYSCALE) {
		glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "effectType"), GRAYSCALE);
		quad->SetTexture(sceneColourTex[0]);
		quad->Draw();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[0], 0);
	}



	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	pType = currentType;

}

void Renderer::PresentScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(screenShader);
	
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	quad->SetBumpMap(0);
	Vector4 colour = Vector4(1, 1, 1, 1);
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&colour);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bloomTex"), 1);
	//SetActiveTextures or Set quad texture
	quad->SetTexture(sceneColourTex[0]);
	if (post && pType != BLUR) {
		if (pType == SOBEL)
			quad->SetBumpMap(sceneColourTex[2]);
		else if (pType == GRAYSCALE || pType == INVERT)
			quad->SetTexture(sceneColourTex[2]);
		else
			quad->SetBumpMap(sceneColourTex[1]);
	}
		

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
		DrawText("Shift/Space: Increase/Decrease Camera Height", Vector3(0, 40, 0), 16.0f);
		DrawText("R: Toggle Particles", Vector3(0, 60, 0), 16.0f);
		DrawText("1: Scene 1, 2: Scene 2, 3: Scene 3, 4: Split Screen", Vector3(0, 80, 0), 16.0f);
		DrawText("P: Toggle Post Processing", Vector3(0, 100, 0), 16.0f);
		DrawText("Num 1 - 5: Post Processing Effects", Vector3(0, 120, 0), 16.0f);
		DrawText("Q/E: Increase/Decrease Light Height", Vector3(0, 140, 0), 16.0f);
		DrawText("Pause: Cycle Scenes", Vector3(0, 160, 0), 16.0f);
	}
	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::DrawSplitScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Vector4 nodeColour = Vector4(1, 1, 1, 1);
	modelMatrix.ToIdentity();
	SetCurrentShader(splitScreenShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&nodeColour);

	//TOP LEFT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.5f, -0.5f, -0.1))* Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(0);
	quad->Draw();

	//BOTTOM LEFT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.5f, 0.5f, -0.1))*Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(splitColourTex[0]);
	quad->Draw();

	//TOP RIGHT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.5f, -0.5f, -0.1))* Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(splitColourTex[1]);
	quad->Draw();

	//BOTTOM RIGHT
	viewMatrix.ToIdentity();
	viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.5f, 0.5f, -0.1))*Matrix4::Scale(Vector3(0.5f, 0.5f, 0));
	UpdateShaderMatrices();
	quad->SetTexture(splitColourTex[2]);
	quad->Draw();
	glUseProgram(0);
}

void Renderer::ResetCameras()
{
	camera->SetCamera(0, 230, Vector3(-2000.0f, 10000.0f, 2000.0f));
	camera2->SetCamera(0, 320, Vector3(-3000, 1000, 3000));
	camera3->SetCamera(-40, 210, Vector3(-1000, 2000, -2000));
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

void Renderer::SetupScreen()
{
	GenerateScreenTextures(sceneDepthTex, true);
	GenerateScreenTextures(sceneColourTex[0], false);
	GenerateScreenTextures(sceneColourTex[1], false);
	GenerateScreenTextures(sceneColourTex[2], false);

	//Gen FBO
	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneColourTex[0], 0);
	//glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !sceneDepthTex || !sceneColourTex[0])
		return;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::SetupPostProcess()
{
	glGenFramebuffers(1, &processFBO);
}



void Renderer::SetupSplitScreen()
{
	for (int i = 0; i < 3; ++i) {
		GenerateScreenTextures(splitDepthTex[i], true);
		GenerateScreenTextures(splitColourTex[i], false);

		glGenFramebuffers(1, &splitScreenFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, splitScreenFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, splitDepthTex[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, splitColourTex[i], 0);
		//glDrawBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !splitDepthTex[i] || !splitColourTex[i])
			return;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
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