#pragma once

#include "OGLRenderer.h"

//Shader types;
#define SHADER_VERTEX	0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY	2

using namespace std;

class Shader
{
public:
	Shader(string vertex, string frag, string geom = "");
	~Shader();

	GLuint GetProgram() { return program; }
	bool LinkProgram();

protected:
	void SetDefaultAtrributes();
	bool LoadShaderFile(string from, string &into);
	GLuint GenerateShader(string from, GLenum type);

	GLuint objects[3];
	GLuint program;

	bool loadFailed;

};

