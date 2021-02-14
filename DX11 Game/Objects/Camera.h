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

	const XMMATRIX& GetViewMatrix() const noexcept { return view; }
	const XMMATRIX& GetProjectionMatrix() const noexcept { return projection; }
	const XMFLOAT3& GetCameraTarget() const noexcept { return cameraTarget; }

	const float& GetCameraSpeed() const noexcept { return cameraSpeed; }
	void SetCameraSpeed( float newSpeed ) noexcept { cameraSpeed = newSpeed; }
	void UpdateCameraSpeed( float updateSpeed ) noexcept { cameraSpeed += updateSpeed; }

	const float& GetFoVDegrees() const noexcept { return fovDegrees; }
	const float& GetNearZ() const noexcept { return nearZ; }
	const float& GetFarZ() const noexcept { return farZ; }
private:
	void UpdateMatrix() override;
	XMMATRIX view, projection;
	XMFLOAT3 cameraTarget;
	float cameraSpeed, fovDegrees;
	float nearZ, farZ;
};

#endif