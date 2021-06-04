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

void Camera::UpdateMatrix()
{
	// Update camera target
	XMMATRIX cameraRotation = XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z );
	XMVECTOR camTarget = XMVector3TransformCoord( DEFAULT_FORWARD_VECTOR, cameraRotation );
	camTarget += posVector;

	// Store camera target
	cameraTarget = { XMVectorGetX( camTarget ), XMVectorGetY( camTarget ), XMVectorGetZ( camTarget ) };

	// Calculate up direction vector based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord( DEFAULT_UP_VECTOR, cameraRotation );
	view = XMMatrixLookAtLH( posVector, camTarget, upDir );

	UpdateDirectionVectors();
}