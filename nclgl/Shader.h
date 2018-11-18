#pragma once

#include "OGLRenderer.h"

//Shader types;
#define SHADER_VERTEX	0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY	2
#define SHADER_TCS		3
#define SHADER_TES		4
#define SHADER_COMPUTE	5

using namespace std;

class Shader
{
public:
	Shader(string vertex, string frag, string geom = "", string tcs = "", string tes = "");
	Shader(string compute);
	~Shader();

	GLuint GetProgram() { return program; }
	bool LinkProgram();

protected:
	void SetDefaultAtrributes();
	bool LoadShaderFile(string from, string &into);
	GLuint GenerateShader(string from, GLenum type);

	GLuint objects[5];
	GLuint program;

	bool loadFailed;

};

