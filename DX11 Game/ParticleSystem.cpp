#include "stdafx.h"
#include "ParticleSystem.h"

bool ParticleSystem::Initialize(XMFLOAT3 pos, XMFLOAT3 rot)
{
	SetPosition(pos);
	SetRotation(rot);
	UpdateMatrix();

    return true;
}

bool ParticleSystem::InitializeParticles(ParticleBase particle, int numOfParticles)
{
	if (particles.empty())
	{
		for (int i = 0; i < numOfParticles; i++)
		{
			particles.push_back(&particle);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void ParticleSystem::Draw()
{
	for(ParticleBase* particle : particles)
	{
		particle->Draw();
	}
}

void ParticleSystem::UpdateMatrix()
{
	worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.y) * XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * XMMatrixTranslation(position.x, position.y, position.z);
	UpdateDirectionVectors();
}
