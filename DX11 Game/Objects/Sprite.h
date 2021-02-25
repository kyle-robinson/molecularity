#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "GameObject2D.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexType.h"
#include "Texture.h"

/// <summary>
/// Create a 2D sprite object.
/// Inherits from GameObject2D.h to allow for modification of position/rotation/scale data in 2D space.
/// </summary>
class Sprite : public GameObject2D
{
public:
	bool Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
		float width, float height, std::string spritePath,
		ConstantBuffer<CB_VS_matrix_2D>& cb_vs_vertexshader_2d );
	void Draw( XMMATRIX orthoMatrix );
	float GetWidth() const noexcept { return scale.x; }
	float GetHeight() const noexcept { return scale.y; }
private:
	void UpdateMatrix() override;
	std::unique_ptr<Texture> texture;
	XMMATRIX worldMatrix = XMMatrixIdentity();

	ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d = nullptr;
	ID3D11DeviceContext* context = nullptr;
	VertexBuffer<Vertex2D> vertices;
	IndexBuffer indices;
};

#endif