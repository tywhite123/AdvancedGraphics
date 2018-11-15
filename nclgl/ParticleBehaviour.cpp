#include "ParticleBehaviour.h"



//ParticleBehaviour::ParticleBehaviour()
//{
//	particles.reserve(NO_OF_PARTICLES);
//	for (int i = 0; i < NO_OF_PARTICLES; ++i)
//		particles.push_back(new Particle());
//
//	last = 0;
//	particleCount = 0;
//}

ParticleBehaviour::ParticleBehaviour(int noOfParticles, Vector3 center, Vector4 colour, Vector3 vel, float life)
{
	Mesh* m = Mesh::GenerateQuad();
	particles.reserve(noOfParticles);
	for (int i = 0; i < noOfParticles; ++i)
		particles.push_back(new Particle(Vector3(rand()%10000, center.y, center.z), vel, colour, life, m));
	last = 0;
	particleCount = 0;
	systemCenter = center;
	this->life = life;
}


ParticleBehaviour::~ParticleBehaviour()
{
	for (int i = 0; i < particles.size(); ++i)
		delete particles[i];
}

void ParticleBehaviour::UpdateSystem(float msec)
{
	
	int newParticle = (msec*10000.0);
	if (newParticle > PARTICLES_PER_FRAME_LIMIT)
		newParticle = PARTICLES_PER_FRAME_LIMIT;

	for (int i = 0; i < particles.size(); ++i) {
		if (particles[i]->GetDraw()) {

			//float grav = (particles[i]->GetVelocity().y * msec * 0.5) + -1.0f;
			//if (grav < -0.01f)
			//	grav = -0.01f;


			particles[i]->UpdateLife(msec);
			if (particles[i]->GetLife() >= 0) {
				if (particles[i]->GetDraw()) {
					particles[i]->UpdatePosition(particles[i]->GetVelocity());
				/*	particles[i]->UpdateVelocity(Vector3(0, grav, 0));*/
					
					particles[i]->Update(msec);
				}
			}
			else {
				particles[i]->SetDraw(false);
				particles[i]->SetLife(life);
				float x = (float)rand() / (float)RAND_MAX;
				float z = (float)rand() / (float)RAND_MAX;
				particles[i]->SetPosition(Vector3(systemCenter.y, rand() % 100 + x, rand() % 100 + z));
			}
		}
	}
	

	//for(int i = 0; i < newParticle; ++i) {
		EmitParticle();
	//}

	

	/*for (int i = 0; i < particles.size(); ++i) {
		Particle* p = particles[i];

		p->UpdateLife(msec);
		if (p->GetLife() > 0.0f) {
			p->UpdateVelocity(Vector3(0.0f, -9.81f, 0.0f) * msec * 0.5f);
			p->UpdatePosition(p->GetVelocity()*msec);
			p->SetDraw(true);
		}
		else {
			p->SetDraw(false);

		}

	}

	particleCount++;*/
	//cout << systemCenter << endl;

}

void ParticleBehaviour::Draw(GLuint matrixLoc)
{
	for (int i = 0; i < particles.size(); ++i) {
		if (particles[i]->GetDraw()) {
			glUniformMatrix4fv(matrixLoc, 1, false, (float*)&particles[i]->GetParticleMatrix());
			particles[i]->Draw();
		}
	}
}



int ParticleBehaviour::UnusedParticles()
{
	for (int i = last; i < particles.size(); ++i) {
		if (!particles.at(i)->GetDraw()) {
			last = i;
			return i;
		}
	}
	for (int i = 0; i < particles.size(); ++i) {
		if (!particles.at(i)->GetDraw()) {
			last = i;
			return i;
		}
	}
	return 0;
}

void ParticleBehaviour::EmitParticle()
{
	float dirX = rand() % 10 - 5;
	float dirZ = rand() % 10 - 5;
	float dirY = rand() % 10 - 5;
	int i = UnusedParticles();
	particles[i]->SetVelocity(Vector3(/*dirX*/10, dirY*10, dirZ*10));
	//particles[i]->GetVelocity().Normalise();
	particles[i]->SetDraw(true);

}


