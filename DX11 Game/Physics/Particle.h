#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <DirectXMath.h>
#include <ParticleSystem.h>

using namespace DirectX;

class Particle
{
public:
	Particle(ParticleSystem* system);
	~Particle();

	void Draw();
	void Update(float t);

	XMMATRIX GetPosition() const { return mPosition; }
	void SetPosition(XMMATRIX pos) { mPosition = pos;  }
private:
	ParticleSystem* mParentSystem;
	XMMATRIX mPosition = XMMatrixIdentity();
};

#endif