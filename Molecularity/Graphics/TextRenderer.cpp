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
		XMFLOAT2( 0.0f, 0.0f ), Scale );
	spriteBatch->End();
}

void TextRenderer::RenderCubeMoveText( LevelContainer& level )
{
	for ( uint32_t i = 0; i < level.GetNumOfLevelCubes(); i++ )
	{
		if ( level.GetCube()[i]->GetIsInRange() && level.GetCube()[i]->GetIsHovering() && !level.GetCube()[i]->GetIsHolding() )
		{
			float halfWidth = static_cast<float>( level.GetGraphics()->GetWidth() ) / 2.0f;
			float halfHeight = static_cast<float>( level.GetGraphics()->GetHeight() ) / 2.0f;
			DrawString( L"Press 'E' to pick up cube.", XMFLOAT2( halfWidth, halfHeight ), Colors::LightGreen );
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

// Update viewport when screen size changes
void TextRenderer::UpdateViewPort( D3D11_VIEWPORT& NewView )
{
	spriteBatch->SetViewport( NewView );

	// Scale text
	float xScale = 1, yScale = 1;
	if ( NewView.Width <= 2560 && NewView.Width > 1920 )
		xScale = 2.0f;
	else if ( NewView.Width <= 1920 && NewView.Width > 1600 )
		xScale = 1.5f;
	else if ( NewView.Width <= 1600 && NewView.Width > 1024 )
		xScale = 1.0f;
	else if ( NewView.Width <= 1024 )
		xScale = 0.9f;
	else
		xScale = 1.0f;

	if ( NewView.Height <= 1440 && NewView.Height > 1080 )
		yScale = 2.0f;
	else if ( NewView.Height <= 1080 && NewView.Height > 900 )
		yScale = 1.5f;
	else if ( NewView.Height <= 900 && NewView.Height > 576 )
		yScale = 1.0f;
	else if ( NewView.Height <= 576 )
		yScale = 0.9f;
	else
		yScale = 1.0f;

	SetScale( { xScale,yScale } );
}