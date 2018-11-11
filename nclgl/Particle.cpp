#include "Particle.h"



Particle::Particle(Vector3 pos, Vector3 vel, Vector4 col, float life) :
	position(pos), velocity(vel), colour(col), life(life)
{
	quad = Mesh::GenerateQuad();
	draw = false;
}

Particle::~Particle()
{
}
