#include "stdafx.h"
#include "Particle.h"

Particle::Particle( ParticleSystem* system )
	: mParentSystem( system ), mPosition( mParentSystem->GetPosition() ) { }

void Particle::Draw()
{

}

void Particle::Update( const float dt )
{

}