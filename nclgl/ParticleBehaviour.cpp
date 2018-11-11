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
		particles.push_back(new Particle(center, vel, colour, life, m));
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

			/*float grav = (particles[i]->GetVelocity().y * msec * 0.5) + -1.0f;
			if (grav < -4.8f)
				grav = -4.8f;*/


			particles[i]->UpdateLife(msec);
			if (particles[i]->GetLife() >= 0) {
				if (particles[i]->GetDraw()) {
					particles[i]->UpdatePosition(particles[i]->GetVelocity()/4);
					//particles[i]->UpdateVelocity(Vector3(0, grav, 0));
					
					particles[i]->Update(msec);
				}
			}
			else {
				particles[i]->SetDraw(false);
				particles[i]->SetLife(life);
				particles[i]->SetPosition(systemCenter);
			}
		}

	}
	

	for(int i = 0; i < newParticle; ++i) {
		EmitParticle();
	}

	

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
		else {
			cout << "not" << endl;
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
	float dirX = rand() % 10000 - 5000;
	float dirZ = rand() % 10000 - 5000;
	int i = UnusedParticles();
	particles[i]->SetVelocity(Vector3(dirX/2, 5000, dirZ/2));
	//particles[i]->GetVelocity().Normalise();
	particles[i]->SetDraw(true);

}


