#pragma once
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

/// <summary>
/// Manages the created particle systems.
/// Contains functions to enable/disable systems individually.
/// </summary>

#include "ParticleSystem.h"

class ParticleManager
{
public:
	ParticleManager();
	virtual ~ParticleManager() = default;

	void CreateParticleSystem();
	void RemoveParticleSystem();

	void EnableParticleSystem();
	void DisableParticleSystem();

	std::vector<std::shared_ptr<ParticleSystem>> GetParticleSystems() const noexcept { return mParticleSystems; }
private:
	std::vector<std::shared_ptr<ParticleSystem>> mParticleSystems;
};

#endif