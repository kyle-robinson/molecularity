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
		static enum class Type
		{
			Main,
			Sub
		};
		Viewport( Graphics& gfx, Type type ) : type( type )
		{
			// get render target dimensions
			FLOAT width = static_cast< FLOAT >( gfx.GetWidth() );
			FLOAT height = static_cast< FLOAT >( gfx.GetHeight() );

			// create viewport
			viewportDesc = CD3D11_VIEWPORT( 0.0f, 0.0f, width, height );
			if ( type == Type::Sub )
			{
				//viewportDesc.TopLeftX = width / 1.333f;
				viewportDesc.TopLeftX = 0.0f;
				viewportDesc.Width = width / 4.0f;
				viewportDesc.Height = height / 4.0f;
				viewportDesc.MaxDepth = 0.1f;
				viewportDesc.MinDepth = 0.0f;
			}
		}

		void Bind( Graphics& gfx ) noexcept override
		{
			GetContext( gfx )->RSSetViewports( 1u, &viewportDesc );
		}
	private:
		Type type;
		CD3D11_VIEWPORT viewportDesc = { };
	};
}

#endif