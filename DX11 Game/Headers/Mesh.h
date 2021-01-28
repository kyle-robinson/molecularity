#pragma once
#ifndef MESH_H
#define MESH_H

#include "Texture.h"
#include "VertexType.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh
{
public:
	Mesh( ID3D11Device* device,
		ID3D11DeviceContext* context,
		std::vector<Vertex3D>& vertices,
		std::vector<WORD>& indices,
		std::vector<Texture>& textures,
		const DirectX::XMMATRIX& transformMatrix );
	const DirectX::XMMATRIX& GetTransformMatrix();
	Mesh( const Mesh& mesh );
	void Draw();
private:
	VertexBuffer<Vertex3D> vertexBuffer;
	IndexBuffer indexBuffer;
	ID3D11DeviceContext* context;
	std::vector<Texture> textures;
	DirectX::XMMATRIX transformMatrix;
};

#endif