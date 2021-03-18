#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include "Particle.h"
#include "GameObject.h"

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(GameObject* parent, int maxParticles);
	ParticleSystem(int maxParticles);
	~ParticleSystem();

	void Draw();
	void Update(float t);

	void CreateParticles(ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix> cb, int num, std::string filePath, XMFLOAT3 rotation, XMFLOAT3 scale, XMFLOAT3 color, float mass, float lifeSpan);

	std::vector<Particle> GetParticles() const { return mParticles;  }
	std::shared_ptr<GameObject> GetParent() const { return mParent;  }
private:
	//components
	std::shared_ptr<GameObject> mParent;
	std::vector<Particle> mParticles;

	//properties
	int mMaxParticles;
};

#endif