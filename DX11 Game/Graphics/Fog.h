#pragma once
#ifndef FOG_H
#define FOG_H

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
class Graphics;

class Fog
{
public:
	Fog();
	//Fog( Graphics& gfx, XMFLOAT3 fogColor, float fogStart, float fogEnd );
	bool Initialize( Graphics& gfx );
	void UpdateConstantBuffer( Graphics& gfx ) noexcept;
	void SpawnControlWindow();
private:
	float end;
	float start;
	XMFLOAT3 color;
	BOOL enable = TRUE;
	ConstantBuffer<CB_VS_fog> cb_vs_fog;
};

#endif