#pragma once
#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "GraphicsResource.h"
#include "ErrorLogger.h"

namespace Bind
{
	class Rasterizer : public GraphicsResource
	{
	public:
		Rasterizer( Graphics& gfx, bool isSolid, bool isTwoSided ) : isSolid( isSolid ), isTwoSided( isTwoSided )
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
		Rasterizer( Graphics& gfx, ID3D11RasterizerState* pRasterizer, bool isSolid, bool isTwoSided )
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
		void Bind( Graphics& gfx ) noexcept override
		{
			GetContext( gfx )->RSSetState( pRasterizer.Get() );
		}
		static void DrawSolid( Graphics& gfx, UINT indexCount ) noexcept
		{
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer_Solid;
			GetContext( gfx )->RSSetState( pRasterizer_Solid.Get() );
			GetContext( gfx )->DrawIndexed( indexCount, 0, 0 );
		}
		static void DrawWireframe( Graphics& gfx, UINT indexCount ) noexcept
		{
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer_Wireframe;
			GetContext( gfx )->RSSetState( pRasterizer_Wireframe.Get() );
			GetContext( gfx )->DrawIndexed( indexCount, 0, 0 );
		}
	private:
		bool isSolid;
		bool isTwoSided;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	};
}

#endif