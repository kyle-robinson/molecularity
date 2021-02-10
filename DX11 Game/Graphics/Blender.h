#pragma once
#ifndef BLENDER_H
#define BLENDER_H

#include "GraphicsResource.h"

namespace Bind
{
	class Blender : public GraphicsResource
	{
	public:
		Blender( Graphics& gfx )
		{
			try
			{
				D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = { 0 };
				renderTargetBlendDesc.BlendEnable = TRUE;
				renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
				renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
				renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
				renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
				renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
				renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				D3D11_BLEND_DESC blendDesc = { 0 };
				blendDesc.RenderTarget[0] = renderTargetBlendDesc;
				HRESULT hr = GetDevice( gfx )->CreateBlendState( &blendDesc, blendState.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create Blend State!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		void Bind( Graphics& gfx ) noexcept override
		{
			GetContext( gfx )->OMSetBlendState( blendState.Get(), NULL, 0xFFFFFFFF );
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	};
}

#endif