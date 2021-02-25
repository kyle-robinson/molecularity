#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

class Particle;
#include "Particle.h"
#include "ParticleTransform.h"
#include "RenderableGameObject.h"

/// <summary>
/// Used to create a particle system.
/// Each system can utilize particles with different parameters.
/// </summary>
class ParticleSystem
{
public:
	ParticleSystem( std::shared_ptr<RenderableGameObject> parent, int maxParticles, float particleSize );
	ParticleSystem( XMMATRIX position, int maxParticles, float particleSize );
	virtual ~ParticleSystem() = default;

	void Draw();
	void Update( const float dt );

	XMMATRIX GetPosition() const noexcept { return mPosition; }
	void SetPosition( XMMATRIX pos ) noexcept { mPosition = pos; }

	std::vector<std::shared_ptr<Particle>> GetParticles() const noexcept { return mParticles; }
	std::shared_ptr<GameObject> GetParent() const noexcept { return mParent; }
private:
	std::vector<std::shared_ptr<Particle>> mParticles;
	std::shared_ptr<RenderableGameObject> mParent;
	XMMATRIX mPosition;
};

#endif