#include "HeightMap.h"



HeightMap::HeightMap(string name)
{
	ifstream file(name, ios::binary);
	if (!file)
		return;

	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH - 1)* (RAW_HEIGHT - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numVertices];



}


HeightMap::~HeightMap()
{
}
