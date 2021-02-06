#pragma once
#ifndef MOUSEPICKING_H
#define MOUSEPICKING_H

#include <DirectXMath.h>
using namespace DirectX;

class Graphics;
class GameObject3D;

class MousePicking
{
public:
	void Initialize( XMMATRIX viewMatrix, XMMATRIX projectionMatrix, int width, int height );
	void UpdateMatrices( XMMATRIX viewMatrix, XMMATRIX projectionMatrix );
	bool TestIntersection( int mouseX, int mouseY, GameObject3D object );
private:
	XMMATRIX viewMatrix, projectionMatrix;
	int width, height;
};

#endif