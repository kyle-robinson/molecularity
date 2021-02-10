#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "GraphicsResource.h"

class Graphics;
namespace Bind
{
	class Viewport : public GraphicsResource
	{
	public:
		Viewport( Graphics& gfx ) : Viewport( gfx, gfx.GetWidth(), gfx.GetHeight() ) {}
		Viewport( Graphics& gfx, UINT width, UINT height )
		{
			viewportDesc = CD3D11_VIEWPORT( 0.0f, 0.0f, static_cast<FLOAT>( width ), static_cast<FLOAT>( height ) );
			GetContext( gfx )->RSSetViewports( 1u, &viewportDesc );
		}
	private:
		CD3D11_VIEWPORT viewportDesc = {};
	};
}

#endif