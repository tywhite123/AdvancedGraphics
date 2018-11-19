#pragma once

#include <vector>
#include "Particle.h"
#include "SceneNode.h"

const int MAX_OF_PARTICLES = 10000;
const float PARTICLES_PER_FRAME_LIMIT = 0.1f;//0.016f * 10000.0f;

class ParticleBehaviour : public SceneNode
{
public:
	ParticleBehaviour();
	ParticleBehaviour(int noOfParticles, Vector3 center, Vector4 colour, Vector3 vel, float life);
	~ParticleBehaviour();

	void UpdateSystem(float msec);
	void Draw(GLuint matrixLoc);

	Vector3 GetCenter() { return systemCenter; }

	int UnusedParticles();
	void EmitParticle();

protected:
	void RebufferData();

	Vector3 systemCenter;
	int noOfParticles;
	int last;
	int particleCount;
	float life;
	
	float newParticle;

	vector<Particle*> particles;
};

