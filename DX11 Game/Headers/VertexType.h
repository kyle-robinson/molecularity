#pragma once
#ifndef VERTEXTYPE_H
#define VERTEXTYPE_H

#include <DirectXMath.h>

struct Vertex_Pos_Col
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

#endif