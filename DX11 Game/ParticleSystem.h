#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Objects/GameObject3D.h"
#include "ParticleBase.h"

class ParticleSystem : public GameObject3D
{
public:
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 rot);
	bool InitializeParticles(ParticleBase particle, int maxParticles);
	void Draw();

protected:
	void UpdateMatrix() override;

	std::list<ParticleBase*> particles;
	XMMATRIX worldMatrix = XMMatrixIdentity();
};

#endif