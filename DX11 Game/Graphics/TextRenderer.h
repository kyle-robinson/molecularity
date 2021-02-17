#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "GraphicsResource.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
using namespace DirectX;

class Graphics;

class TextRenderer : public GraphicsResource
{
public:
	TextRenderer( GraphicsContainer& gfx, const std::wstring& fileName );
	void RenderCubeMoveText( Graphics& gfx );
	void RenderMultiToolText( Graphics& gfx );
	void RenderCameraText( Graphics& gfx );
private:
	void DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color );
private:
	std::unique_ptr<SpriteFont> spriteFont;
	std::unique_ptr<SpriteBatch> spriteBatch;
};

#endif