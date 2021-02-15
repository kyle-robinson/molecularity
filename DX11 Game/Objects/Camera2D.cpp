#include "Camera2D.h"

Camera2D::Camera2D()
{
	position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	posVector = XMLoadFloat3( &position );
	rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	rotVector = XMLoadFloat3( &rotation );
	UpdateMatrix();
}

void Camera2D::SetProjectionValues( float width, float height, float nearZ, float farZ )
{
	orthoMatrix = XMMatrixOrthographicOffCenterLH( 0.0f, width, height, 0.0f, nearZ, farZ );
}

const XMMATRIX& Camera2D::GetOrthoMatrix() const noexcept
{
	return orthoMatrix;
}

const XMMATRIX& Camera2D::GetWorldMatrix() const noexcept
{
	return worldMatrix;
}

const XMMATRIX& Camera2D::GetWorldOrthoMatrix() const noexcept
{
	return worldMatrix * orthoMatrix;
}

void Camera2D::UpdateMatrix()
{
	XMMATRIX translationOffsetMatrix = XMMatrixTranslation( -position.x, -position.y, 0.0f );
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z );
	worldMatrix = cameraRotationMatrix * translationOffsetMatrix;
}