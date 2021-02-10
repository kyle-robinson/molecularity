#pragma once
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
class Camera;

class SpotLight : public Light
{
public:
	void const SetLightPosition( const DirectX::XMFLOAT3& pos ) noexcept { spotLightPosition = pos; };
	void const SetLightDirection( const DirectX::XMFLOAT3& dir ) noexcept { spotLightDirection = dir; };
	const DirectX::XMFLOAT3& const GetLightPosition() const noexcept { return spotLightPosition; };
	void UpdateConstantBuffer( ConstantBuffer<CB_PS_spot>& cb_ps_spot, std::unique_ptr<Camera>& camera );
	void SpawnControlWindow();
private:
	float cone = 20.0f;
	float range = 1000.0f;

	float innerCutoff = 1.0f;
	float outerCutoff = 20.0f;
	DirectX::XMFLOAT3 spotLightColor = { 1.0f, 0.5f, 1.0f };
	DirectX::XMFLOAT3 spotLightPosition = { -5.0f, 9.0f, -10.0f };
	DirectX::XMFLOAT3 spotLightDirection = { 0.0f, 0.0f, 1.0f };
};

#endif