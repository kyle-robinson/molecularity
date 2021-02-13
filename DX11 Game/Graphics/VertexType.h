#pragma once
#ifndef VERTEXTYPE_H
#define VERTEXTYPE_H

#include <DirectXMath.h>

struct Vertex3D
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT3 normals;
};

struct Vertex2D
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
};

struct Vertex_Pos_Col
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

struct Vertex_Pos_Tex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
};

#endif