#include "stdafx.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::CreateParticleSystem(GameObject* parent, int maxParticles)
{
	auto system = make_shared<ParticleSystem>(parent, maxParticles);
	mParticleSystems.push_back(system);
}

void ParticleManager::CreateParticleSystem(int maxParticles)
{
	auto system = make_shared<ParticleSystem>(maxParticles);
	mParticleSystems.push_back(system);
}

void ParticleManager::RemoveParticleSystem()
{

}

void ParticleManager::EnableParticleSystem()
{

}

void ParticleManager::DisableParticleSystem()
{

}
