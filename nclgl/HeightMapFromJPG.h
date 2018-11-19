#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include "Mesh.h"

#define RAW_WIDTH 257
#define RAW_HEIGHT 257

#define HEIGHTMAP_X 32.0f
#define HEIGHTMAP_Z 32.0f
#define HEIGHTMAP_Y /*0.0f*/20.0f
#define HEIGHTMAP_TEX_X 1.0f / HEIGHTMAP_X
#define HEIGHTMAP_TEX_Z 1.0f / HEIGHTMAP_Z

class HeightMapFromJPG : public Mesh
{
public:
	HeightMapFromJPG(string name);
	~HeightMapFromJPG();
};

