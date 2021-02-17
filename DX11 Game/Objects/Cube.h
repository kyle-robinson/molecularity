#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "RenderableGameObject.h"
#include "CubeProperties.h"

class Cube : public RenderableGameObject
{
public:
	bool Initialize( ID3D11DeviceContext* context, ID3D11Device* device );
	void Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ID3D11ShaderResourceView* texture ) noexcept;

	CubeProperties* GetEditableProperties() const noexcept { return editableProperties; }

private:
	CubeProperties* editableProperties;
	ID3D11DeviceContext* context;
	VertexBuffer<Vertex3D> vb_cube;
	IndexBuffer ib_cube;
};

#endif