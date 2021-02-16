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
	Fog( XMFLOAT3 fogColor, float fogStart, float fogEnd );
	void UpdateConstantBuffer( ConstantBuffer<CB_VS_fog>& cb_vs_fog ) noexcept;
	void SpawnControlWindow();
private:
	float end;
	float start;
	XMFLOAT3 color;
	BOOL enable = TRUE;
};

#endif