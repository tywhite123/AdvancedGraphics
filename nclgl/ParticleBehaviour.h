#pragma once

#include <vector>
#include "Particle.h"
#include "SceneNode.h"

const int NO_OF_PARTICLES = 500;
const int PARTICLES_PER_FRAME_LIMIT = 0.016f * 10000.0f;

class ParticleBehaviour : public SceneNode
{
public:
	ParticleBehaviour();
	ParticleBehaviour(int noOfParticles, Vector3 center, Vector4 colour, Vector3 vel, float life);
	~ParticleBehaviour();

	void UpdateSystem(float msec);
	void Draw();

	int UnusedParticles();
	void EmitParticle();

protected:
	Vector3 systemCenter;
	int noOfParticles;
	int last;
	int particleCount;
	float life;

	

	vector<Particle*> particles;
};
