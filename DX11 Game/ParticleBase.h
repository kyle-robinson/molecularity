#pragma once
#ifndef PARTICLEBASE_H
#define PARTICLEBASE_H

#include "Objects/RenderableGameObject.h"

class ParticleBase : public RenderableGameObject
{
public:
	bool Initialize(ID3D11DeviceContext* context, ID3D11Device* device);
	void Draw();
	void Update(const float deltaTime) noexcept;
private:
	int delay;

	XMFLOAT3 prevPos;
	XMFLOAT3 pos;
};

#endif