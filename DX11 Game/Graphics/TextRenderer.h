#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

class Graphics;
#include "GraphicsResource.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>

/// <summary>
/// Uses SpriteFont and SpriteBatch to draw strings to the screen.
/// Font to be used can be chagned within this classes constructor.
/// Contains functions which render text relevant to multi-tool/camera/cube objects.
/// </summary>
class TextRenderer : public GraphicsResource
{
public:
	TextRenderer( GraphicsContainer& gfx );
	TextRenderer(std::string Font, ID3D11Device* device, ID3D11DeviceContext* contex);
	void RenderCubeMoveText( Graphics& gfx );
	void RenderMultiToolText( Graphics& gfx );
	void RenderCameraText( Graphics& gfx );

	void RenderString(std::string text, XMFLOAT2 position, XMVECTORF32 color);
	void UpdateViewPort(D3D11_VIEWPORT& NewView);
private:
	void DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color );
private:
	std::wstring fileName;
	std::unique_ptr<SpriteFont> spriteFont;
	std::unique_ptr<SpriteBatch> spriteBatch;
};

#endif