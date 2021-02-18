#pragma once
#ifndef LIGHT_H
#define LIGHT_H

/// <summary>
/// Container class for other light objects.
/// Inherits from RenderableGameObject.h to enable lights to bind a model to their position.
/// </summary>

#include "RenderableGameObject.h"

class Light : public RenderableGameObject
{
protected:
	bool Initialize( const std::string& filePath, ID3D11Device* device,
		ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix );
};

#endif