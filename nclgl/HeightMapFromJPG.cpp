#include "HeightMapFromJPG.h"



HeightMapFromJPG::HeightMapFromJPG(string name)
{



	//numVertices = RAW_WIDTH * RAW_HEIGHT;
	//numIndices = (RAW_WIDTH - 1)* (RAW_HEIGHT - 1) * 6;
	//vertices = new Vector3[numVertices];
	//textureCoords = new Vector2[numVertices];
	//indices = new GLuint[numIndices];

	//int width = 1081;

	//unsigned char* data;
	//data = SOIL_load_image(name.c_str(), &width, &width, 0, SOIL_LOAD_RGB);

	//for (int x = 0; x < RAW_WIDTH; ++x) {
	//	for (int z = 0; z < RAW_HEIGHT; ++z) {

	//		//Set the offset for the Y coords and which coord to apply data too
	//		int offset = (((x * RAW_WIDTH) + z) * 3)-1;

	//		//vertices[offset] = Vector3(x*HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);
	//		vertices[offset] = Vector3(x*HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);

	//		textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);

	//	}
	//}

	//delete data;

	//numIndices = 0;

	//for (int x = 0; x < RAW_WIDTH - 1; ++x) {
	//	for (int z = 0; z < RAW_HEIGHT - 1; ++z) {

	//		int a = (x * (RAW_WIDTH)) + z;
	//		int b = ((x + 1) * (RAW_WIDTH)) + z;
	//		int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
	//		int d = (x * (RAW_WIDTH)) + (z + 1);

	//		indices[numIndices++] = c;
	//		indices[numIndices++] = b;
	//		indices[numIndices++] = a;

	//		indices[numIndices++] = a;
	//		indices[numIndices++] = d;
	//		indices[numIndices++] = c;

	//	}
	//}

	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH - 1)* (RAW_HEIGHT - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	unsigned char *data;
	int width = 1081;
	data = SOIL_load_image(name.c_str(), &width, &width, 0, SOIL_LOAD_RGB);
	float pctX, pctY, dx, dy;

	for (int i = 0; i < RAW_WIDTH; ++i) {
		for (int j = 0; j < RAW_HEIGHT; ++j) {
			pctX = float(i) / (RAW_WIDTH / float(width - 1));
			pctY = float(j) / (RAW_WIDTH / float(width  - 1));
			dx = pctX - int(pctX);
			dy = pctY - int(pctY);

			vertices[i*RAW_WIDTH + j].x = i*HEIGHTMAP_X;
			vertices[i*RAW_WIDTH + j].z = j*HEIGHTMAP_Z;
			vertices[i*RAW_WIDTH+j].y = 
				((
					data[3 * ((int(pctX)) * width + int(pctY))] * (1 - dx)        * (1 - dy)
					+ data[3 * ((int(pctX + 1)) * width + int(pctY))] * dx            * (1 - dy)
					+ data[3 * ((int(pctX)) * width + int(pctY + 1))] * (1 - dx)    * dy
					+ data[3 * ((int(pctX + 1)) * width + int(pctY + 1))] * dx        * dy))*HEIGHTMAP_Y;

			textureCoords[i*RAW_WIDTH + j] = Vector2(i * HEIGHTMAP_TEX_X, j * HEIGHTMAP_TEX_Z);
		}

	}

	delete data;

	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) {

			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;

		}
	}

	GenerateNormals();
	GenerateTangents();
	BufferData();
}

HeightMapFromJPG::~HeightMapFromJPG()
{
}
