#pragma once
#ifndef STENCILOUTLINE_H
#define STENCILOUTLINE_H

#include "Stencil.h"
#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

class Cube;
class RenderableGameObject;

/// <summary>
/// Uses the stencil buffer to create a stencil outline on a given game object.
/// Provides functions to spawn ImGui control window and change outline parameters.
/// </summary>
class StencilOutline : public GraphicsResource
{
public:
	StencilOutline( GraphicsContainer& gfx );
	void DrawWithOutline( GraphicsContainer& gfx, Cube& cube, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix,
		ConstantBuffer<CB_PS_point>& cb_ps_point, ID3D11ShaderResourceView* texture );
	void DrawWithOutline( GraphicsContainer& gfx, RenderableGameObject& object, ConstantBuffer<CB_PS_point>& cb_ps_point );
	void SpawnControlWindow();

	void SetOutlineScale( float outlineScale ) noexcept { scale = outlineScale; }
	void SetOutlineColor( XMFLOAT3 outlineColor ) noexcept { color = outlineColor; }
private:
	float scale;
	XMFLOAT3 color;
	PixelShader pixelShader_outline;
	VertexShader vertexShader_outline;
	ConstantBuffer<CB_PS_outline> cb_ps_outline;
};

#endif