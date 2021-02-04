#include "Light.h"

//"Flashlight" (https://skfb.ly/6QXJG) by Brandon Baldwin is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

bool Light::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader )
{
	if ( !model.Initialize( "Resources\\Models\\Flashlight.fbx", device, context, cb_vs_vertexshader ) )
		return false;
	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	UpdateMatrix();
	return true;
}