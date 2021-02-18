#pragma once
#ifndef VERTEXTYPE_H
#define VERTEXTYPE_H

/// <summary>
/// Contains structs for reference by VertexBuffer.h to determine the type of vertices it is to be given.
/// This shader to be used must reflect the VertexType that is templated on in VertexBuffer.h
/// </summary>

#include <DirectXMath.h>
using namespace DirectX;

struct Vertex3D
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
	XMFLOAT3 normals;
};

struct Vertex2D
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
};

struct Vertex_Pos_Col
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct Vertex_Pos_Tex
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
};

#endif