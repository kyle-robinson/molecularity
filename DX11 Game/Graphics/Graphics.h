#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Quad.h"
#include "Sprite.h"
#include "Camera.h"
#include "Camera2D.h"
#include "GraphicsContainer.h"
#include <dxtk/WICTextureLoader.h>

class Graphics : public GraphicsContainer
{
public:
	// FUNCTIONS //
	enum ResizeScale { SMALL, NORMAL, LARGE } resizeScale = LARGE;
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, int width, int height );

	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( float dt );
	
	Cube& GetCube() noexcept { return cube; }
	std::unique_ptr<Camera>& GetCamera( const std::string& cam ) noexcept { return cameras[cam]; }
private:
	bool InitializeScene();
	void RenderSkySphere();
	void RenderLights();
	void RenderPrimitives();
	void RenderSprites();
public:
	// VARIABLES //
	float outlineScale = 0.1f;
	bool rasterizerSolid = true;
	std::string selectedBox = "Basic";
	XMFLOAT3 outlineColor = { 1.0f, 0.6f, 0.1f };
private:
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallNormalTexture;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;

	Cube cube;
	Quad simpleQuad;
	Sprite crosshair;
	Camera2D camera2D;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;
};

#endif