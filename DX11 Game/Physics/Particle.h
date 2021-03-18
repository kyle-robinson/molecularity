#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "RenderableGameObject.h"
#include "Timer.h"
#include "ParticleSystem.h"

using namespace DirectX;

class Particle : public RenderableGameObject
{
public:
	Particle(ParticleSystem* parentSystem, XMFLOAT3 color, float mass, float lifeSpan);
	~Particle();

	void Draw();
	void Update(float t);

	void EnableParticle() { mIsDisabled = false; }
	void DisableParticle() { mIsDisabled = true; }
	void ResetParticlePosition();

private:
	//components
	std::shared_ptr<GameObject> mParent;

	//properties
	XMFLOAT3 mColor;
	float mLifeSpan;
	Timer mCurrentLife;
	bool mIsDisabled = false;
};

#endif