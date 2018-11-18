#include "Shader.h"




Shader::Shader(string vertex, string frag, string geom, string tcs, string tes)
{
	program = glCreateProgram();
	objects[SHADER_VERTEX] = GenerateShader(vertex, GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT] = GenerateShader(frag, GL_FRAGMENT_SHADER);

	if (!geom.empty()) {
		objects[SHADER_GEOMETRY] = GenerateShader(geom, GL_GEOMETRY_SHADER);
		glAttachShader(program, objects[SHADER_GEOMETRY]);
	}
	if (!tcs.empty() && !tes.empty()) {
		
	}

	glAttachShader(program, objects[SHADER_VERTEX]);
	glAttachShader(program, objects[SHADER_FRAGMENT]);
	SetDefaultAtrributes();
}

Shader::Shader(string compute)
{
	program = glCreateProgram();
	//objects[SHADER_COMPUTE] ...

	//glAttachShader(program, objects[SHADER_COMPUTE]);
}

Shader::~Shader()
{
	for (int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

bool Shader::LinkProgram()
{
	if (loadFailed)
		return false;
	
	glLinkProgram(program);

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	GLchar error[8192] = {};
	
	if (code != GL_TRUE) {
		glGetShaderInfoLog(program, 8192, NULL, error);
		cout << "Error linking: " << error << endl;
	}


	return code == GL_TRUE ? true : false;
}

void Shader::SetDefaultAtrributes()
{
	glBindAttribLocation(program, VERTEX_BUFFER, "position");
	glBindAttribLocation(program, COLOUR_BUFFER, "colour");
	glBindAttribLocation(program, NORMAL_BUFFER, "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
}



bool Shader::LoadShaderFile(string from, string & into)
{
	ifstream file;
	string temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if (!file.is_open()) {
		cout << "File doesnt exist!" << endl;
		return false;
	}

	while (!file.eof()) {
		getline(file, temp);
		into += temp + "\n";
	}

	file.close();
	cout << into << endl << endl;
	cout << "Loaded Shader Text!" << endl << endl;
	return true;
}

GLuint Shader::GenerateShader(string from, GLenum type)
{
	cout << "Compiling Shader ..." << endl;

	//Try to load the shader
	string load;
	if (!LoadShaderFile(from, load)) {
		cout << "Compiling Failed!" << endl;
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);


	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		cout << "Compiling Failed!" << endl;
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		loadFailed = true;
		return 0;
	}

	cout << "Compiling Success!" << endl;
	loadFailed = false;
	return shader;
}
