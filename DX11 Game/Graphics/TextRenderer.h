#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "Graphics.h"
#include "GraphicsResource.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include <memory>

namespace Bind
{
	class TextRenderer : public GraphicsResource
	{
	public:
		TextRenderer( GraphicsContainer& gfx, const std::wstring& fileName )
		{
			spriteBatch = std::make_unique<SpriteBatch>( GetContext( gfx ) );
			spriteFont = std::make_unique<SpriteFont>( GetDevice( gfx ),
				std::wstring( L"Resources\\Fonts\\" ).append( fileName ).c_str() );
		}
		void RenderCubeMoveText( GraphicsContainer& gfx )
		{
			if ( gfx.cubeInRange && gfx.cubeHover && !gfx.holdingCube )
			{
				DrawString( L"Press 'E' to pick up cube.",
					XMFLOAT2( gfx.GetWidth() / 2 - 120.0f, gfx.GetHeight() / 2 - 40.0f ), Colors::LightGreen );
			}
		}
		void RenderMultiToolText( Graphics& gfx )
		{
			if ( gfx.GetCube().GetEditableProperties()->GetType() == ToolType::CONVERT )
			{
				DrawString( L"Multi-Tool: CONVERT", XMFLOAT2( gfx.GetWidth() - 760.0f, 0.0f ), Colors::White );

				static std::wstring boxType;
				switch ( gfx.boxToUse )
				{
				case 0: boxType = L"Default Box"; break;
				case 1: boxType = L"Bounce Box"; break;
				case 2: boxType = L"Jump Box"; break;
				case 3: boxType = L"TNT Box"; break;
				}

				DrawString( std::wstring( L"Texture: " ).append( boxType ).c_str(),
					XMFLOAT2( gfx.GetWidth() - 260.0f, 0.0f ), Colors::Orange );
			}
			else if ( gfx.GetCube().GetEditableProperties()->GetType() == ToolType::RESIZE )
			{
				DrawString( L"Multi-Tool: RESIZE", XMFLOAT2( gfx.GetWidth() - 760.0f, 0.0f ), Colors::White );

				static std::wstring sizeType;
				switch ( gfx.sizeAmount )
				{
				case 0: sizeType = L"Shrink Ray"; break;
				case 1: sizeType = L"Reset Ray"; break;
				case 2: sizeType = L"Growth Ray"; break;
				}

				DrawString( std::wstring( L"Size: " ).append( sizeType ).c_str(),
					XMFLOAT2( gfx.GetWidth() - 260.0f, 0.0f ), Colors::BlueViolet );
			}
		}
		void RenderCameraText( GraphicsContainer& gfx )
		{
			DrawString( std::wstring( L"Camera: " ).append( StringConverter::StringToWide( gfx.cameraToUse ) ).c_str(),
				XMFLOAT2( 20.0f, 0.0f ), Colors::IndianRed );
		}
	private:
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