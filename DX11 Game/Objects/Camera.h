#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject3D.h"
#include "RenderableGameObject.h"

class Camera : public GameObject3D
{
public:
	Camera( const XMFLOAT3& initialPosition );
	Camera( float xPos, float yPos, float zPos );
	Camera& operator=( Camera );
	void SetProjectionValues( float fovDegrees, float aspectRatio, float nearZ, float farZ );

	const XMMATRIX& GetViewMatrix() const noexcept;
	const XMMATRIX& GetProjectionMatrix() const noexcept;
	const XMFLOAT3& GetCameraTarget() const noexcept;

	const float& GetCameraSpeed() const noexcept;
	void SetCameraSpeed( float newSpeed ) noexcept;
	void UpdateCameraSpeed( float updateSpeed ) noexcept;

	const float& GetFoVDegrees() const noexcept;
	const float& GetNearZ() const noexcept;
	const float& GetFarZ() const noexcept;
private:
	void UpdateMatrix() override;
	XMMATRIX view, projection;
	XMFLOAT3 cameraTarget;
	float cameraSpeed, fovDegrees;
	float nearZ, farZ;
};

#endif