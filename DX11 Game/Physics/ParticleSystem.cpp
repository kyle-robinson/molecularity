#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GameObject* parent, int maxParticles) : mParent(parent), mMaxParticles(maxParticles)
{
	
}

ParticleSystem::ParticleSystem(int maxParticles) : mMaxParticles(maxParticles)
{
	mParent = nullptr;
}

ParticleSystem::~ParticleSystem()
{
	
}

void ParticleSystem::Draw()
{
	for(auto &particle : mParticles)
	{
		particle.Draw();
	}
}

void ParticleSystem::Update(float t)
{
	for (auto &particle : mParticles)
	{
		particle.Update(t);
	}
}

void ParticleSystem::CreateParticles(ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix> cb, int num, std::string filePath, XMFLOAT3 rotation, XMFLOAT3 scale, XMFLOAT3 color, float mass, float lifeSpan)
{
	for (int i = 0; i < num; i++)
	{
		if (mParticles.size() >= mMaxParticles)
		{
			return;
		}

		Particle particle = Particle(this, color, mass, lifeSpan);
		particle.Initialize(filePath, device, context, cb);
		particle.SetInitialPosition(this->GetPositionFloat3());
		particle.SetInitialRotation(rotation);
		particle.SetInitialScale(scale);

		mParticles.push_back(particle);
	}
}