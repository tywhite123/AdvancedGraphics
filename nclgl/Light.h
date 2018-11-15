#pragma once

#include "Vector4.h"

enum LightType {
	DIRECTIONAL, SPOT
};

class Light
{
public:
	Light(Vector3 position, Vector4 colour, float radius, LightType type) {
		this->position = position;
		this->colour = colour;
		this->radius = radius;
		lightType = type;
	}
	Light() {};

	~Light();

	void UpdateLight(float msec);

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	LightType GetType() const { return lightType; }
	void SetType(LightType val) { lightType = val; }

protected:
	Vector3 position;
	Vector4 colour;
	float radius;
	LightType lightType;

};

