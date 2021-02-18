#pragma once
#ifndef DEPTHSTENCIL_H
#define DEPTHSTENCIL_H

/// <summary>
/// Creates the depth stencil/z-buffer for the application.
/// Call "ClearDepthStencil( gfx )" in the render pipeline to clear the depth buffer.
/// </summary>

#include "GraphicsResource.h"
#include "ErrorLogger.h"

namespace Bind
{
	class DepthStencil : public GraphicsResource
	{
	public:
		DepthStencil( GraphicsContainer& gfx ) : DepthStencil( gfx, static_cast<FLOAT>( gfx.GetWidth() ),
			static_cast<FLOAT>( gfx.GetHeight() ) ) { }
		DepthStencil( GraphicsContainer& gfx, float width, float height )
		{
			try
			{
				CD3D11_TEXTURE2D_DESC depthStencilDesc( DXGI_FORMAT_D24_UNORM_S8_UINT,
					static_cast<UINT>( width ), static_cast<UINT>( height ) );
				depthStencilDesc.MipLevels = 1;
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

				Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
				HRESULT hr = GetDevice( gfx )->CreateTexture2D( &depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil texture!" );
				hr = GetDevice( gfx )->CreateDepthStencilView( depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil view!" );

				CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = CD3D11_DEPTH_STENCIL_DESC( CD3D11_DEFAULT{} );
				depthStencilStateDesc.DepthEnable = TRUE;
				depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				hr = GetDevice( gfx )->CreateDepthStencilState( &depthStencilStateDesc, depthStencilState.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil state!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		void ClearDepthStencil( GraphicsContainer& gfx ) noexcept
		{
			GetContext( gfx )->ClearDepthStencilView( depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
			GetContext( gfx )->OMSetDepthStencilState( depthStencilState.Get(), NULL );
		}
		ID3D11DepthStencilView* GetDepthStencilView() noexcept
		{
			return depthStencilView.Get();
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	};
}

#endif