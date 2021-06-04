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
#include <vector>

/// <summary>
/// Load indiviual mesh components for a given model and store data in appropriate vertex/index buffers.
/// Transformation matrices are applied iteratively to properly update a meshes position relevant to other meshes of the same object.
/// </summary>
using namespace DirectX;

class Mesh
{
public:
	Mesh( ID3D11Device* device,
		ID3D11DeviceContext* context,
		std::vector<Vertex3D>& vertices,
		std::vector<WORD>& indices,
		std::vector<Texture>& textures,
		const XMMATRIX& transformMatrix );
	const XMMATRIX& GetTransformMatrix();
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