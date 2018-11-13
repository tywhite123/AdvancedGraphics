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
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, 
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	virtual void Draw(); //Virtual so it can be implemented or overloaded
	static Mesh* GenerateTriangle();  //Static so that it can be independant of any object. Returns a pointer to a mesh;
	static Mesh* GenerateQuad();
	static Mesh* GeneratePoint();


	void GenerateNormals();

	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }


	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }


protected:
	void BufferData();
	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
						const Vector3 &c, const Vector2 &ta,
						const Vector2 &tb, const Vector2 &tc);

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER]; //Set size to max buffer which is its value in enum
	GLuint numVertices;
	GLuint numIndices;
	GLuint type;
	GLuint texture;
	GLuint bumpTexture;
	
	unsigned int* indices;
	Vector3* vertices;
	Vector4* colours;
	Vector2* textureCoords;
	Vector3* normals;
	Vector3* tangents;

	bool particle;



};

