#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

/// <summary>
/// Create a viewport on which to project the current scene.
/// Can be modified to allow for split-screen functionality.
/// </summary>

#include "GraphicsResource.h"

class Graphics;
namespace Bind
{
	class Viewport : public GraphicsResource
	{
	public:
		Viewport( GraphicsContainer& gfx ) : Viewport( gfx, gfx.GetWidth(), gfx.GetHeight() ) { }
		Viewport( GraphicsContainer& gfx, UINT width, UINT height )
		{
			viewportDesc = CD3D11_VIEWPORT( 0.0f, 0.0f, static_cast<FLOAT>( width ), static_cast<FLOAT>( height ) );
			GetContext( gfx )->RSSetViewports( 1u, &viewportDesc );
		}
	private:
		CD3D11_VIEWPORT viewportDesc = {};
	};
}

#endif