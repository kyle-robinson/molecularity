#pragma once
#ifndef CUBE_H
#define CUBE_H

/// <summary>
/// Create a simple cube object.
/// Inherits from RenderableGameObject which allows for the individual setting of position/rotation/scale data.
/// </summary>

#include "RenderableGameObject.h"
#include "CubeProperties.h"
#include "PhysicsModel.h"

class Cube : public RenderableGameObject
{
public:
	bool Initialize( ID3D11DeviceContext* context, ID3D11Device* device );
	void Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ID3D11ShaderResourceView* texture ) noexcept;
	
	void UpdatePhysics( const float deltaTime ) noexcept;
	std::shared_ptr<CubeProperties> GetEditableProperties() const noexcept { return editableProperties; }
private:
	std::shared_ptr<CubeProperties> editableProperties;
	std::shared_ptr<PhysicsModel> physicsModel;
	ID3D11DeviceContext* context;
	VertexBuffer<Vertex3D> vb_cube;
	IndexBuffer ib_cube;
};

#endif