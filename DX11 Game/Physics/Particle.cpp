#include "stdafx.h"
#include "Particle.h"

Particle::Particle(ParticleSystem* system) : mParentSystem(system)
{
	mPosition = mParentSystem->GetPosition();
}

Particle::~Particle()
{

}

void Particle::Draw()
{

}

void Particle::Update(float t)
{

}
