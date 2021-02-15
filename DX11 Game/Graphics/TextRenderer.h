#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "GraphicsResource.h"
#include <memory>
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>

namespace Bind
{
	class TextRenderer : public GraphicsResource
	{
	public:
		TextRenderer( Graphics& gfx, const std::wstring& fileName )
		{
			spriteBatch = std::make_unique<SpriteBatch>( GetContext( gfx ) );
			spriteFont = std::make_unique<SpriteFont>( GetDevice( gfx ),
				std::wstring( L"Resources\\Fonts\\" ).append( fileName ).c_str() );
		}
		void DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color )
		{
			spriteBatch->Begin();
			spriteFont->DrawString( spriteBatch.get(), text.c_str(), position, color, 0.0f,
				XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) );
			spriteBatch->End();
		}
	private:
		std::unique_ptr<SpriteFont> spriteFont;
		std::unique_ptr<SpriteBatch> spriteBatch;
	};
}

#endif