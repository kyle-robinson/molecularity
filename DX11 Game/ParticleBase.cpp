#include "stdafx.h"
#include "ParticleBase.h"
#include "Physics/PhysicsModel.h"

bool ParticleBase::Initialize(ID3D11DeviceContext* context, ID3D11Device* device)
{
	return false;
}

void ParticleBase::Draw()
{
	RenderableGameObject::Draw();
}

void ParticleBase::Update(const float deltaTime) noexcept
{
	physicsModel->Update(deltaTime / 20.0f);

	// update positioning
	pos = GetPositionFloat3();

	if (pos.x != prevPos.x || pos.z != prevPos.z)
		physicsModel->AddForce(XMFLOAT3((pos.x - prevPos.x) * 5.0f, 0.0f, (pos.z - prevPos.z) * 5.0f));

	if (delay == 5)
		prevPos = pos;
	delay++;
	if (delay > 5)
		delay = 0;
}