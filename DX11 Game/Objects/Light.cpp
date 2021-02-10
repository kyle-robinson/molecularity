#include "Light.h"

bool Light::Initialize( const std::string& filePath, ID3D11Device* device,
	ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix )
{
	if ( !model.Initialize( filePath.c_str(), device, context, cb_vs_matrix ) )
		return false;
	UpdateMatrix();
	return true;
}