#pragma once
#ifndef MOUSEPICKING_H
#define MOUSEPICKING_H

/// <summary>
/// Used to test for mouse intersections with game objects in the scene.
/// Must be initialized with appropriate camera view and projection matrices.
/// Camera view and projection matrices must be updated each frame and the camera in use may change.
/// </summary>

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