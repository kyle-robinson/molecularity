#include "Camera.h"

Camera::Camera( const XMFLOAT3& initialPosition )
{
	SetInitialPosition( initialPosition );
	posVector = XMLoadFloat3( &initialPosition );
	rotation = { 0.0f, 0.0f, 0.0f };
	rotVector = XMLoadFloat3( &rotation );
	UpdateMatrix();
}

Camera::Camera( float xPos, float yPos, float zPos ) : Camera( XMFLOAT3( xPos, yPos, zPos ) ) { }

Camera& Camera::operator=( Camera )
{
	return *this;
}

void Camera::SetProjectionValues( float fovDegrees, float aspectRatio, float nearZ, float farZ )
{
	this->fovDegrees = fovDegrees;
	this->nearZ = nearZ;
	this->farZ = farZ;
	float fovRadians = ( fovDegrees / 360.0f ) * XM_2PI;
	projection = XMMatrixPerspectiveFovLH( fovRadians, aspectRatio, nearZ, farZ );
}

const XMMATRIX& Camera::GetViewMatrix() const noexcept
{
	return view;
}

const XMMATRIX& Camera::GetProjectionMatrix() const noexcept
{
	return projection;
}

const XMFLOAT3& Camera::GetCameraTarget() const noexcept
{
	return cameraTarget;
}

const float& Camera::GetCameraSpeed() const noexcept
{
	return cameraSpeed;
}

void Camera::SetCameraSpeed( float newSpeed ) noexcept
{
	cameraSpeed = newSpeed;
}

void Camera::UpdateCameraSpeed( float updateSpeed ) noexcept
{
	cameraSpeed += updateSpeed;
}

const float& Camera::GetFoVDegrees() const noexcept
{
	return fovDegrees;
}

const float& Camera::GetNearZ() const noexcept
{
	return nearZ;
}

const float& Camera::GetFarZ() const noexcept
{
	return farZ;
}

void Camera::UpdateMatrix()
{
	// update camera target
	XMMATRIX cameraRotation = XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z );
	XMVECTOR camTarget = XMVector3TransformCoord( DEFAULT_FORWARD_VECTOR, cameraRotation );
	camTarget += posVector;

	cameraTarget = { XMVectorGetX( camTarget ), XMVectorGetY( camTarget ), XMVectorGetZ( camTarget ) };

	// calculate up direction vector based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord( DEFAULT_UP_VECTOR, cameraRotation );
	view = XMMatrixLookAtLH( posVector, camTarget, upDir );

	UpdateDirectionVectors();
}