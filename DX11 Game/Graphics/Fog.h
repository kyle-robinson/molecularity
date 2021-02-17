#pragma once
#ifndef FOG_H
#define FOG_H

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
class GraphicsContainer;

class Fog
{
public:
	Fog();
	bool Initialize( GraphicsContainer& gfx );
	void UpdateConstantBuffer( GraphicsContainer& gfx ) noexcept;
	void SpawnControlWindow();
private:
	float end;
	float start;
	XMFLOAT3 color;
	BOOL enable = TRUE;
	ConstantBuffer<CB_VS_fog> cb_vs_fog;
};

#endif