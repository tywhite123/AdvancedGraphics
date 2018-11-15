#pragma once

#include "Vector4.h"
#include "Mesh.h"
#include "Shader.h"

class Particle 
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector4 col, float life, Mesh*& m);
	~Particle();


	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 pos) { 
		position = pos; 
		particleMatrix = Matrix4::Translation(Vector3(position));
	}
	void UpdatePosition(Vector3 pos) { position += pos; }

	Vector3 GetVelocity() const { return velocity; }
	void SetVelocity(Vector3 vel) { 
		velocity = vel; 
		//velocity.Normalise(); 
	}
	void UpdateVelocity(Vector3 vel) { velocity += vel; }


	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 col) { colour = col; }


	float GetLife() const { return life; }
	void SetLife(float life) { this->life = life; }
	void UpdateLife(float life) { this->life -= life; }

	void SetDraw(bool d) { draw = d; }
	bool GetDraw() const { return draw; }
	void Draw();
	void Update(float msec);
	Matrix4 GetParticleMatrix() { return particleMatrix; }
	





protected:
	Mesh* quad;
	Matrix4 particleMatrix;
	Vector3 position;
	Vector3 velocity;
	Vector4 colour;
	float life;

	bool draw;


};

