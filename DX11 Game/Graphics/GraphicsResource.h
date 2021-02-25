#pragma once
#ifndef GRAPHICSRESOURCE_H
#define GRAPHICSRESOURCE_H

#include "GraphicsContainer.h"

/// <summary>
/// Creates function interfaces for bindables to use without needing direct access to GraphicsContainer.h
/// When creating a bindable, "Bind( gfx )" should be overridden and used to bind that component to the render pipeline.
/// Is a 'friend' of GraphicsContainer.h allowing this class access to its private and protected member variables/functions.
/// </summary>
class GraphicsResource
{
protected:
	static std::shared_ptr<Bind::Stencil> GetStencil( GraphicsContainer& gfx, const std::string& stencil ) noexcept { return gfx.stencils[stencil]; }

	static Microsoft::WRL::ComPtr<ID3D11Device>& GetDeviceRef( GraphicsContainer& gfx ) noexcept { return gfx.device; }
	static ID3D11DeviceContext* GetContext( GraphicsContainer& gfx ) noexcept { return gfx.context.Get(); }
	static ID3D11Device* GetDevice( GraphicsContainer& gfx ) noexcept { return gfx.device.Get(); }

	virtual void Bind( GraphicsContainer& gfx ) noexcept { UNREFERENCED_PARAMETER( gfx ); }
	virtual ~GraphicsResource() = default;
};

#endif