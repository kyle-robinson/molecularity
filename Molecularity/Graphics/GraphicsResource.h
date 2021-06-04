#pragma once
#ifndef GRAPHICSRESOURCE_H
#define GRAPHICSRESOURCE_H

#include "Graphics.h"

/// <summary>
/// Creates function interfaces for bindables to use without needing direct access to Graphics.h
/// When creating a bindable, "Bind( gfx )" should be overridden and used to bind that component to the render pipeline.
/// Is a 'friend' of Graphics.h allowing this class access to its private and protected member variables/functions.
/// </summary>
class GraphicsResource
{
protected:
	static std::shared_ptr<Bind::Stencil> GetStencil( Graphics& gfx, const std::string& stencil ) noexcept { return gfx.stencils[stencil]; }

	static Microsoft::WRL::ComPtr<ID3D11Device>& GetDeviceRef( Graphics& gfx ) noexcept { return gfx.device; }
	static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept { return gfx.context.Get(); }
	static ID3D11Device* GetDevice( Graphics& gfx ) noexcept { return gfx.device.Get(); }

	virtual void Bind( Graphics& gfx ) noexcept { UNREFERENCED_PARAMETER( gfx ); }
	virtual ~GraphicsResource() = default;
};

#endif