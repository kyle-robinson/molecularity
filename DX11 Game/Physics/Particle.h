#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "ParticleSystem.h"
class ParticleSystem;

/// <summary>
/// Create, modify and render a particle.
/// Positions are derived from given particle system.
/// </summary>
class Particle
{
public:
	Particle( ParticleSystem* system );
	virtual ~Particle() = default;

	void Draw();
	void Update( const float dt );

	XMMATRIX GetPosition() const noexcept { return mPosition; }
	void SetPosition( XMMATRIX pos ) noexcept { mPosition = pos;  }
private:
	std::shared_ptr<ParticleSystem> mParentSystem;
	XMMATRIX mPosition = XMMatrixIdentity();
};

#endif