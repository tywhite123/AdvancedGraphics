#include "Renderer1.h"




Renderer1::Renderer1(Window & parent) : OGLRenderer(parent)
{
	triangle = Mesh::GenerateTriangle();

	currentShader = new Shader(SHADERDIR"basicVertex.glsl", SHADERDIR"colourFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}

	init = true;
}

Renderer1::~Renderer1()
{
	delete triangle;
}

void Renderer1::RenderScene()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	triangle->Draw();
	glUseProgram(0);

	SwapBuffers();

}
