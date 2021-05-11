#include "stdafx.h"
#include "TextRenderer.h"
#include "Graphics.h"

TextRenderer::TextRenderer( Graphics& gfx ) : fileName( L"open_sans_ms_16.spritefont" )
{
	spriteBatch = std::make_unique<SpriteBatch>( GetContext( gfx ) );
	spriteFont = std::make_unique<SpriteFont>( GetDevice( gfx ),
		std::wstring( L"Resources\\Fonts\\" ).append( fileName ).c_str() );
}

TextRenderer::TextRenderer( std::string Font, ID3D11Device* device, ID3D11DeviceContext* contex )
{
	fileName = std::wstring( Font.begin(), Font.end() );
	spriteBatch = std::make_unique<SpriteBatch>( contex );
	spriteFont = std::make_unique<SpriteFont>( device,
		std::wstring( L"Resources\\Fonts\\" ).append( fileName ).c_str() );
}


void TextRenderer::DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color )
{
	spriteBatch->Begin();
	spriteFont->DrawString( spriteBatch.get(), text.c_str(), position, color, 0.0f,
		XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) );
	spriteBatch->End();
}

void TextRenderer::RenderCubeMoveText( LevelContainer& level )
{
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		if ( level.GetCube()[i]->GetIsInRange() && level.GetCube()[i]->GetIsHovering() && !level.GetCube()[i]->GetIsHolding() )
		{
			DrawString( L"Press 'E' to pick up cube.",
				XMFLOAT2( level.GetGraphics()->GetWidth() / 2 - 120.0f, level.GetGraphics()->GetHeight() / 2 - 40.0f ), Colors::LightGreen );
		}
	}
}

void TextRenderer::RenderMultiToolText( LevelContainer& level )
{
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		if ( level.GetCube()[i]->GetEditableProperties()->GetToolType() == ToolType::Convert )
		{
			DrawString( L"Multi-Tool: CONVERT", XMFLOAT2( level.GetGraphics()->GetWidth() - 760.0f, 0.0f ), Colors::White );

			static std::wstring boxType;
			switch ( level.GetCube()[i]->GetEditableProperties()->GetMaterialID() )
			{
			case 0: boxType = L"Default Box"; break;
			case 1: boxType = L"Bounce Box"; break;
			case 2: boxType = L"Jump Box"; break;
			case 3: boxType = L"TNT Box"; break;
			}

			DrawString( std::wstring( L"Texture: " ).append( boxType ).c_str(),
				XMFLOAT2( level.GetGraphics()->GetWidth() - 260.0f, 0.0f ), Colors::Orange );
		}
		else if ( level.GetCube()[i]->GetEditableProperties()->GetToolType() == ToolType::Resize )
		{
			DrawString( L"Multi-Tool: RESIZE", XMFLOAT2( level.GetGraphics()->GetWidth() - 760.0f, 0.0f ), Colors::White );

			static std::wstring sizeType;
			switch ( level.GetCube()[i]->GetEditableProperties()->GetSizeID() )
			{
			case 0: sizeType = L"Shrink Ray"; break;
			case 1: sizeType = L"Reset Ray"; break;
			case 2: sizeType = L"Growth Ray"; break;
			}

			DrawString( std::wstring( L"Size: " ).append( sizeType ).c_str(),
				XMFLOAT2( level.GetGraphics()->GetWidth() - 260.0f, 0.0f ), Colors::BlueViolet );
		}
	}
}

void TextRenderer::RenderCameraText( LevelContainer& manager )
{
	std::string displayText;

	switch ( manager.GetCameraController()->GetCurrentCamera() )
	{
	case JSON::CameraType::Default: displayText = "Default"; break;
	case JSON::CameraType::Debug: displayText = "Debug"; break;
	default: displayText = "Camera name needed in TextRenderer.h"; break;
	}

	DrawString( std::wstring( L"Camera: " ).append( StringConverter::StringToWide( displayText ) ).c_str(),
		XMFLOAT2( 20.0f, 0.0f ), Colors::IndianRed );
}

void TextRenderer::RenderString( std::string text, XMFLOAT2 position, XMVECTORF32 color )
{
	std::wstring String = std::wstring( text.begin(), text.end() );
	DrawString( String, position, color );
}

// update viewport when screen size changes
void TextRenderer::UpdateViewPort( D3D11_VIEWPORT& NewView )
{
	spriteBatch->SetViewport( NewView );
}