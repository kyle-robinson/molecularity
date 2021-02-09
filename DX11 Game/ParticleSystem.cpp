#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(RenderableGameObject* parent) : mParent(parent)
{
	
}

ParticleSystem::ParticleSystem(XMMATRIX position) : mPosition(position)
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
