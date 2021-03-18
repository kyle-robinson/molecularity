#pragma once
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <vector>
#include "ParticleSystem.h"

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void CreateParticleSystem(GameObject* parent, int maxParticles);
	void CreateParticleSystem(int maxParticles);
	void RemoveParticleSystem();

	void EnableParticleSystem();
	void DisableParticleSystem();

	std::vector<std::shared_ptr<ParticleSystem>> GetParticleSystems() const { return mParticleSystems; }
private:
	std::vector<std::shared_ptr<ParticleSystem>> mParticleSystems;
};

#endif