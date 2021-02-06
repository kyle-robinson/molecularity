#pragma once
#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "GraphicsResource.h"
#include "ErrorLogger.h"

namespace Bind
{
	class DepthStencil;
	class RenderTarget : public GraphicsResource
	{
	public:
		RenderTarget( Graphics& gfx, IDXGISwapChain* swapChain )
		{
			try
			{
				Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
				HRESULT hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )pBackBuffer.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create swap chain!" );
				hr = GetDevice( gfx )->CreateRenderTargetView( pBackBuffer.Get(), nullptr, backBuffer.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create render target view!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		void BindAsBuffer( Graphics& gfx, DepthStencil* depthStencil, float clearColor[4] ) noexcept
		{
			GetContext( gfx )->OMSetRenderTargets( 1, backBuffer.GetAddressOf(), depthStencil->GetDepthStencilView() );
			GetContext( gfx )->ClearRenderTargetView( backBuffer.Get(), clearColor );
		}
		void BindAsNull( Graphics& gfx ) noexcept
		{
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> nullRenderTarget = nullptr;
			GetContext( gfx )->OMSetRenderTargets( 1, nullRenderTarget.GetAddressOf(), nullptr );
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nullShaderResourceView = nullptr;
			GetContext( gfx )->PSSetShaderResources( 0, 1, nullShaderResourceView.GetAddressOf() );
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer;
	};
}

#endif