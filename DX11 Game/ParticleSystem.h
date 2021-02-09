#pragma once
#include <vector>
#include <Particle.h>
#include <Objects/RenderableGameObject.h>

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(RenderableGameObject* parent);
	ParticleSystem(XMMATRIX position);
	~ParticleSystem();

	void Draw();
	void Update(float t);

	XMMATRIX GetPosition() const { return mPosition; }
	void SetPosition(XMMATRIX pos) { mPosition = pos; }

	vector<Particle> GetParticles() const { return mParticles;  }
	GameObject* GetParent() const { return mParent;  }
private:
	vector<Particle> mParticles;
	RenderableGameObject* mParent;
	XMMATRIX mPosition;
};