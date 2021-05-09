#pragma once
#ifndef FOG_H
#define FOG_H

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "GraphicsResource.h"

/// <summary>
/// Create and initialize a simple fog system.
/// Provides functions to update constant buffer and spawn ImGui control window.
/// </summary>
class Fog : public GraphicsResource
{
public:
	Fog( Graphics& gfx );
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