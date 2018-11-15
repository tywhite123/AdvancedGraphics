#pragma once
#include "Light.h"
class DirectionLight :
	public Light
{
public:
	DirectionLight(Vector3 direction, Vector4 colour);
	~DirectionLight();
};

