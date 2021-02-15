#pragma once
#ifndef GRAPHICSRESOURCE_H
#define GRAPHICSRESOURCE_H

#include "GraphicsContainer.h"

class GraphicsResource
{
protected:
	static std::shared_ptr<Bind::Stencil> GetStencil( GraphicsContainer& gfx, const std::string& stencil ) noexcept { return gfx.stencils[stencil]; }
	
	static ConstantBuffer<CB_PS_point>& GetLightCB( GraphicsContainer& gfx ) noexcept { return gfx.cb_ps_point; }
	static VertexShader& GetLightVS( GraphicsContainer& gfx ) noexcept { return gfx.vertexShader_light; }
	static PixelShader& GetLightPS( GraphicsContainer& gfx ) noexcept { return gfx.pixelShader_light; }

	static Microsoft::WRL::ComPtr<ID3D11Device>& GetDeviceRef( GraphicsContainer& gfx ) noexcept { return gfx.device; }
	static ID3D11DeviceContext* GetContext( GraphicsContainer& gfx ) noexcept { return gfx.context.Get(); }
	static ID3D11Device* GetDevice( GraphicsContainer& gfx ) noexcept { return gfx.device.Get(); }

	virtual void Bind( GraphicsContainer& gfx ) noexcept { UNREFERENCED_PARAMETER( gfx ); }
	virtual ~GraphicsResource() = default;
};

#endif