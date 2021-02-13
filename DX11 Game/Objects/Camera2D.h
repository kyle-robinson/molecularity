#pragma once
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "GameObject2D.h"

class Camera2D : public GameObject2D
{
public:
	Camera2D();
	void SetProjectionValues( float width, float height, float nearZ, float farZ );
	const XMMATRIX& GetOrthoMatrix() const noexcept;
	const XMMATRIX& GetWorldMatrix() const noexcept;
	const XMMATRIX& GetWorldOrthoMatrix() const noexcept;
private:
	void UpdateMatrix() override;
	XMMATRIX orthoMatrix;
	XMMATRIX worldMatrix;
};

#endif