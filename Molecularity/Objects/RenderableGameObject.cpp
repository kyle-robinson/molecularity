#include "RenderableGameObject.h"
#include "PhysicsModel.h"
#include "Camera.h"

bool RenderableGameObject::Initialize(
	const std::string& filePath,
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader )
{
	if ( !model.Initialize( filePath, device, context, cb_vs_vertexshader ) )
		return false;

	physicsModel = std::make_shared<PhysicsModel>( this );

	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	UpdateMatrix();
	return true;
}

void RenderableGameObject::Draw()
{
	model.Draw( worldMatrix );
}

void RenderableGameObject::UpdateMatrix()
{
	worldMatrix = XMMatrixScaling( scale.x, scale.y, scale.y ) *
		XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *
		XMMatrixTranslation( position.x, position.y, position.z );
	UpdateDirectionVectors();
}