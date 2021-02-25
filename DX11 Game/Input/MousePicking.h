#pragma once
#ifndef MOUSEPICKING_H
#define MOUSEPICKING_H

class Camera;
class Graphics;
class GameObject3D;

/// <summary>
/// Used to test for mouse intersections with game objects in the scene.
/// Must be initialized with appropriate camera view and projection matrices.
/// Camera view and projection matrices must be updated each frame and the camera in use may change.
/// </summary>
class MousePicking
{
public:
	void Initialize( int width, int height );
	void UpdateMatrices( std::unique_ptr<Camera>& camera );
	bool TestIntersection( int mouseX, int mouseY, GameObject3D object );
private:
	XMMATRIX viewMatrix, projectionMatrix;
	int width, height;
};

#endif