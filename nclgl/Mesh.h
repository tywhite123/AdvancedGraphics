#pragma once

#include "OGLRenderer.h"

//For selecting which buffer
/*
VERTEX BUFFER = 0;
COLOUR BUFFER = 1;
MAX BUFFER = 2;
Max buffer is at the end to define the size of the array;
*/
enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, INDEX_BUFFER, MAX_BUFFER
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	virtual void Draw(); //Virtual so it can be implemented or overloaded
	static Mesh* GenerateTriangle();  //Static so that it can be independant of any object. Returns a pointer to a mesh;
	static Mesh* GenerateQuad();

	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }

protected:
	void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER]; //Set size to max buffer which is its value in enum
	GLuint numVertices;
	GLuint numIndices;
	GLuint type;
	GLuint texture;
	
	unsigned int* indices;
	Vector3* vertices;
	Vector4* colours;
	Vector2* textureCoords;



};

