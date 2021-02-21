#pragma once
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <vector>
#include <ParticleSystem.h>

using namespace std;

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void CreateParticleSystem();
	void RemoveParticleSystem();

	void EnableParticleSystem();
	void DisableParticleSystem();

	vector<ParticleSystem*> GetParticleSystems() const { return mParticleSystems; }
private:
	vector<ParticleSystem*> mParticleSystems;
};

#endif