#include "Light.h"

// "Flashlight" (https://skfb.ly/6QXJG) by Brandon Baldwin is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

bool Light::Initialize( const std::string& filePath, ID3D11Device* device,
	ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix )
{
	if ( !model.Initialize( filePath.c_str(), device, context, cb_vs_matrix ) )
		return false;
	UpdateMatrix();
	return true;
}