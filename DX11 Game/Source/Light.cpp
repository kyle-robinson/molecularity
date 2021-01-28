#include "Light.h"

bool Light::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader )
{
	if ( !model.Initialize( "Resources\\Models\\light.fbx", device, context, cb_vs_vertexshader ) )
		return false;
	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	UpdateMatrix();
	return true;
}