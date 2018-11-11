#pragma once

#include "Vector4.h"
#include "Mesh.h"
#include "Shader.h"

class Particle 
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector4 col, float life);
	~Particle();


	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 pos) { position = pos; }
	void UpdatePosition(Vector3 pos) { position += pos; }

	Vector3 GetVelocity() const { return velocity; }
	void SetVelocity(Vector3 vel) { velocity = vel; }
	void UpdateVelocity(Vector3 vel) { velocity += vel; }


	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 col) { colour = col; }


	float GetLife() const { return life; }
	void SetLife(float life) { this->life = life; }
	void UpdateLife(float life) { this->life -= life; }

	void SetDraw(bool d) { draw = d; }
	bool GetDraw() const { return draw; }
	void Draw() { quad->Draw(); }





protected:
	Mesh* quad;
	Vector3 position;
	Vector3 velocity;
	Vector4 colour;
	float life;

	bool draw;


};

