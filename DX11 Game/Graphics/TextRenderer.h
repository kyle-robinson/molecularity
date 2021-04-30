#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "Level.h"
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
	TextRenderer( Graphics& gfx );
	void RenderCubeMoveText( Level& level );
	void RenderMultiToolText( Level& level );
	void RenderCameraText( Level& level );
private:
	void DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color );
private:
	std::wstring fileName;
	std::unique_ptr<SpriteFont> spriteFont;
	std::unique_ptr<SpriteBatch> spriteBatch;
};

#endif