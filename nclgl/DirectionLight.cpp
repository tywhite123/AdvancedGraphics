#include "DirectionLight.h"





DirectionLight::DirectionLight(Vector3 direction, Vector4 colour)
	: Light(direction, colour, 0)
{
}

DirectionLight::~DirectionLight()
{
}
