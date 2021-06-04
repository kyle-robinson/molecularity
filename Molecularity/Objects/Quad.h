#pragma once
#ifndef QUAD_H
#define QUAD_H

#include "Shaders.h"
#include "RenderableGameObject.h"

/// <summary>
/// Create simple quad object.
/// Inherits from RenderableGameObject which allows for the individual setting of position/rotation/scale data.
/// </summary>
class Quad : public RenderableGameObject
{
public:
	bool Initialize( ID3D11DeviceContext* context, ID3D11Device* device );
	void Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ConstantBuffer<CB_PS_scene>& cb_ps_scene,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* textureNormal ) noexcept;
private:
	ID3D11DeviceContext* context;
	VertexBuffer<Vertex3D> vb_plane;
	IndexBuffer ib_plane;
};

/// <summary>
/// Create a quad using the dimensions of the given window.
/// </summary>
class QuadFullscreen : public RenderableGameObject
{
public:
	bool Initialize( ID3D11Device* device );
	void SetupBuffers( ID3D11DeviceContext* context,
		ConstantBuffer<CB_VS_fullscreen>& cb_vs_full, BOOL multiView ) noexcept;
	IndexBuffer ib_full;
private:
	VertexBuffer<Vertex_Pos> vb_full;
};

#endif