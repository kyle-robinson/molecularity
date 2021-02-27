#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "GraphicsResource.h"

namespace Bind
{
	/// <summary>
	/// Create a viewport on which to project the current scene.
	/// Can be modified to allow for split-screen functionality.
	/// </summary>
	class Viewport : public GraphicsResource
	{
	public:
		Viewport( GraphicsContainer& gfx )
		{
			// get render target dimensions
			FLOAT width = static_cast<FLOAT>( gfx.GetWidth() );
			FLOAT height = static_cast<FLOAT>( gfx.GetHeight() );
			
			// create viewport
			viewportDesc = CD3D11_VIEWPORT( 0.0f, 0.0f, width, height );
			GetContext( gfx )->RSSetViewports( 1u, &viewportDesc );
		}
	private:
		CD3D11_VIEWPORT viewportDesc = { };
	};
}

#endif