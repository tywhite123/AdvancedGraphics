#include "Particle.h"



Particle::Particle(Vector3 pos, Vector3 vel, Vector4 col, float life, Mesh*& m) :
	position(pos), velocity(vel), colour(col), life(life)
{
	particleMatrix = Matrix4::Translation(pos);
	quad = m;
	draw = false;
}

Particle::~Particle()
{
}

void Particle::Draw()
{
	quad->Draw();
}

void Particle::Update(float msec)
{
	particleMatrix = Matrix4::Translation(position);
}
