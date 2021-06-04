#pragma once
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "GameObject2D.h"

/// <summary>
/// Creates a 2D camera which creates an orthographic view on the scene.
/// Used primarily for rendering sprites such as UI components.
/// </summary>
class Camera2D : public GameObject2D
{
public:
	Camera2D();
	void SetProjectionValues( float width, float height, float nearZ, float farZ );
	const XMMATRIX& GetOrthoMatrix() const noexcept;
	const XMMATRIX& GetWorldMatrix() const noexcept;
	const XMMATRIX& GetWorldOrthoMatrix() const noexcept;

	void SendWorldOrthoMatrix();

private:
	void UpdateMatrix() override;
	XMMATRIX orthoMatrix;
	XMMATRIX worldMatrix;

	XMFLOAT4X4 WorldOrthoMatrix;
	
};

#endif