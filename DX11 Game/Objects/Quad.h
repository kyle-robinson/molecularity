#pragma once
#ifndef QUAD_H
#define QUAD_H

/// <summary>
/// Create simple quad object.
/// Inherits from RenderableGameObject which allows for the individual setting of position/rotation/scale data.
/// </summary>

#include "RenderableGameObject.h"

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

#endif