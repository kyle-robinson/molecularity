#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(RenderableGameObject* parent, int maxParticles, float particleSize) : mParent(parent)
{
	for (int i = 0; i < maxParticles; i++)
	{
		mParticles.push_back(new Particle(this));
	}
}

ParticleSystem::ParticleSystem(XMMATRIX position, int maxParticles, float particleSize) : mPosition(position)
{
	mParent = nullptr;

	for (int i = 0; i < maxParticles; i++)
	{
		mParticles.push_back(new Particle(this));
	}
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Draw()
{
	for(auto &particle : mParticles)
	{
		particle->Draw();
	}
}

void ParticleSystem::Update(float t)
{
	for (auto &particle : mParticles)
	{
		particle->Update(t);
	}
}
