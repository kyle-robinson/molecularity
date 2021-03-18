#include "stdafx.h"
#include "Particle.h"

Particle::Particle(ParticleSystem* parentSystem, XMFLOAT3 color, float mass, float lifeSpan) : mParent(parentSystem), mColor(color), mLifeSpan(lifeSpan)
{
	ResetParticlePosition();
	physicsModel = std::make_shared<PhysicsModel>(this);
	mCurrentLife = Timer();
	mCurrentLife.Start();
}

Particle::~Particle()
{
	
}

void Particle::Draw()
{
	if (!mIsDisabled)
	{
		RenderableGameObject::Draw();
	}
}

void Particle::Update(float t)
{
	if (!mIsDisabled)
	{
		RenderableGameObject::UpdateMatrix();
		
		if (mCurrentLife.GetMilliSecondsElapsed() > mLifeSpan)
		{
			ResetParticlePosition();
			mCurrentLife.Restart();
		}
	}
}

void Particle::ResetParticlePosition()
{
	position = mParent->GetPositionFloat3();
}
