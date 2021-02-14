#pragma once
#ifndef GRAPHICSRESOURCE_H
#define GRAPHICSRESOURCE_H

#include "Graphics.h"

class GraphicsResource
{
protected:
	static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept { return gfx.context.Get(); };
	static ID3D11Device* GetDevice( Graphics& gfx ) noexcept { return gfx.device.Get(); };
	virtual void Bind( Graphics& gfx ) noexcept { UNREFERENCED_PARAMETER( gfx ); };
	virtual ~GraphicsResource() = default;
};

#endif