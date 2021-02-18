#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <Particle.h>
#include <Objects/RenderableGameObject.h>
#include <ParticleTransform.h>

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(RenderableGameObject* parent, int maxParticles, float particleSize);
	ParticleSystem(XMMATRIX position, int maxParticles, float particleSize);
	~ParticleSystem();

	void Draw();
	void Update(float t);

	XMMATRIX GetPosition() const { return mPosition; }
	void SetPosition(XMMATRIX pos) { mPosition = pos; }

	vector<Particle*> GetParticles() const { return mParticles;  }
	GameObject* GetParent() const { return mParent;  }
private:
	vector<Particle*> mParticles;
	RenderableGameObject* mParent;
	XMMATRIX mPosition;
};

#endif