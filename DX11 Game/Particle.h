#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Particle
{
public:
	Particle();
	~Particle();

	void Draw();
	void Update(float t);

	XMMATRIX GetPosition() const { return mPosition; }
	void SetPosition(XMMATRIX pos) { mPosition = pos;  }
private:
	XMMATRIX mPosition = XMMatrixIdentity();
};