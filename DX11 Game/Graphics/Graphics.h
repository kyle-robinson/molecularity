#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Fog.h"
#include "Cube.h"
#include "Quad.h"
#include "Sprite.h"
#include "Camera.h"
#include "Camera2D.h"
#include "GraphicsContainer.h"
#include <dxtk/WICTextureLoader.h>

class Graphics : public GraphicsContainer
{
	friend class Fog;
public:
	enum ResizeScale { SMALL, NORMAL, LARGE } resizeScale = LARGE; //might move this. It doenst do anything at the moment so left it incase.
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
	void UpdateConstantBuffers();
	void RenderSkySphere();
	void RenderLights();
	void RenderModels();
	void RenderPrimitives();
	void RenderSprites();

	// Variables
	Cube cube;
	Quad simpleQuad;
	Sprite crosshair;
	Camera2D camera2D;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;

	ConstantBuffer<CB_VS_fog> cb_vs_fog;
	ConstantBuffer<CB_PS_spot> cb_ps_spot;
	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;
	ConstantBuffer<CB_PS_directional> cb_ps_directional;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallNormalTexture;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;
};

#endif