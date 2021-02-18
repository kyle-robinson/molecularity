#pragma once
#ifndef RASTERIZER_H
#define RASTERIZER_H

/// <summary>
/// Creates a rasterizer bindable, referenced in GraphicsContainer.h
/// Constructor takes 'bool' values to modify the CULL and FILL modes.
/// Call "Bind( gfx )" on a class object to bind it to the render pipeline.
/// </summary>

#include "GraphicsResource.h"
#include "ErrorLogger.h"

namespace Bind
{
	class Rasterizer : public GraphicsResource
	{
	public:
		Rasterizer( GraphicsContainer& gfx, bool isSolid, bool isTwoSided )
			: isSolid( isSolid ), isTwoSided( isTwoSided )
		{
			try
			{
				CD3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC( CD3D11_DEFAULT{} );
				rasterizerDesc.MultisampleEnable = TRUE;
				rasterizerDesc.FillMode = isSolid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
				rasterizerDesc.CullMode = isTwoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
				HRESULT hr = GetDevice( gfx )->CreateRasterizerState( &rasterizerDesc, pRasterizer.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create rasterizer state!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		Rasterizer( GraphicsContainer& gfx, ID3D11RasterizerState* pRasterizer, bool isSolid, bool isTwoSided )
			: isSolid( isSolid ), isTwoSided( isTwoSided )
		{
			try
			{
				CD3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC( CD3D11_DEFAULT{} );
				rasterizerDesc.MultisampleEnable = TRUE;
				rasterizerDesc.FillMode = isSolid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
				rasterizerDesc.CullMode = isTwoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
				HRESULT hr = GetDevice( gfx )->CreateRasterizerState( &rasterizerDesc, &pRasterizer );
				COM_ERROR_IF_FAILED( hr, "Failed to create static rasterizer state!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		void Bind( GraphicsContainer& gfx ) noexcept override
		{
			GetContext( gfx )->RSSetState( pRasterizer.Get() );
		}
	private:
		bool isSolid;
		bool isTwoSided;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	};
}

#endif