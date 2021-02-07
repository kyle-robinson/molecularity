#pragma once
#ifndef STENCIL_H
#define STENCIL_H

#include "GraphicsResource.h"

namespace Bind
{
	class Stencil : public GraphicsResource
	{
	public:
		enum class Mode
		{
			Off,
			Mask,
			Write
		};
		Stencil( Graphics& gfx, Mode mode ) : mode( mode )
		{
			try
			{
				CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc( CD3D11_DEFAULT{} );
				
				if ( mode == Mode::Off )
				{
					depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				}
				else if ( mode == Mode::Mask )
				{
					depthStencilStateDesc.DepthEnable = FALSE;
					depthStencilStateDesc.StencilEnable = TRUE;
					depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
					depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
					depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
				}
				else if ( mode == Mode::Write )
				{
					depthStencilStateDesc.StencilEnable = TRUE;
					depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
					depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
					depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
					depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
					depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				}

				HRESULT hr = GetDevice( gfx )->CreateDepthStencilState( &depthStencilStateDesc, pStencil.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil state!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		void Bind( Graphics& gfx ) noexcept override
		{
			GetContext( gfx )->OMSetDepthStencilState( pStencil.Get(), 0 );
		}
	private:
		Mode mode;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pStencil;
	};
}

#endif