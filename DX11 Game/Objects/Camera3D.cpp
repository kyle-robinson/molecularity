#include "Camera3D.h"

Camera3D::Camera3D( const XMFLOAT3& initialPosition )
{
	SetInitialPosition( initialPosition );
	posVector = XMLoadFloat3( &initialPosition );
	rotation = { 0.0f, 0.0f, 0.0f };
	rotVector = XMLoadFloat3( &rotation );
	UpdateMatrix();
}

Camera3D::Camera3D( float xPos, float yPos, float zPos ) : Camera3D( XMFLOAT3( xPos, yPos, zPos ) ) { }

Camera3D& Camera3D::operator=( Camera3D )
{
	return *this;
}

void Camera3D::SetProjectionValues( float fovDegrees, float aspectRatio, float nearZ, float farZ )
{
	this->fovDegrees = fovDegrees;
	this->nearZ = nearZ;
	this->farZ = farZ;
	float fovRadians = ( fovDegrees / 360.0f ) * XM_2PI;
	projection = XMMatrixPerspectiveFovLH( fovRadians, aspectRatio, nearZ, farZ );
}

const XMMATRIX& Camera3D::GetViewMatrix() const noexcept
{
	return view;
}

const XMMATRIX& Camera3D::GetProjectionMatrix() const noexcept
{
	return projection;
}

const float& Camera3D::GetCameraSpeed() const noexcept
{
	return cameraSpeed;
}

void Camera3D::SetCameraSpeed( float newSpeed ) noexcept
{
	cameraSpeed = newSpeed;
}

void Camera3D::UpdateCameraSpeed( float updateSpeed ) noexcept
{
	cameraSpeed += updateSpeed;
}

const float& Camera3D::GetFoVDegrees() const noexcept
{
	return fovDegrees;
}

const float& Camera3D::GetNearZ() const noexcept
{
	return nearZ;
}

const float& Camera3D::GetFarZ() const noexcept
{
	return farZ;
}

void Camera3D::ResetOrientation() noexcept
{
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
}

void Camera3D::ResetProjection( float aspectRatio ) noexcept
{
	fovDegrees = 70.0f;
	nearZ = 0.1f;
	farZ = 1000.0f;
	SetProjectionValues( fovDegrees, aspectRatio, nearZ, farZ );
}

void Camera3D::UpdateThirdPerson( std::shared_ptr<Camera3D>& camera, GameObject3D& model ) noexcept
{
	XMFLOAT3 viewing = model.GetPositionFloat3();
    viewing.x -= 2.5f;
    viewing.y += 13.0f;
    camera->SetLookAtPos( viewing );
        
    XMFLOAT3 rotations = model.GetRotationFloat3();
    rotations.x = -rotations.x;
    rotations.y += XMConvertToRadians( 180.0f );
    rotations.z = -rotations.z;
	camera->SetRotation( rotations );

    static XMFLOAT3 leftPos, backPos;
    XMFLOAT3 positions = model.GetPositionFloat3();
    XMStoreFloat3( &leftPos, model.GetBackwardVector() * -5.0f );
    XMStoreFloat3( &backPos, model.GetLeftVector() * 2.5f );
    positions.x += leftPos.x + backPos.x;
    positions.y += leftPos.y + backPos.y + 13.0f;
    positions.z += leftPos.z + backPos.z;

	camera->SetPosition( positions );
}

void Camera3D::UpdateMatrix()
{
	// update camera target
	XMMATRIX cameraRotation = XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z );
	XMVECTOR camTarget = XMVector3TransformCoord( DEFAULT_FORWARD_VECTOR, cameraRotation );
	camTarget += posVector;

	// calculate up direction vector based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord( DEFAULT_UP_VECTOR, cameraRotation );
	view = XMMatrixLookAtLH( posVector, camTarget, upDir );

	UpdateDirectionVectors();
}