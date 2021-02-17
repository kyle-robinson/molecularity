#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Fog.h"
#include "Cube.h"
#include "Quad.h"
#include "Camera.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
#include "GraphicsContainer.h"
#include <dxtk/WICTextureLoader.h>

class StencilOutline;
class TextRenderer;

class Graphics : public GraphicsContainer
{
	friend class Application;
	friend class ImGuiManager;
public:
	// Functions
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

public:
	// Variables
	int boxToUse = 0;
	int sizeAmount = 2;
	bool cubeHover = false;
	bool cubeInRange = false;
	bool holdingCube = false;
	std::string cameraToUse = "Default";
private:
	float sizeToUse = 1.0f;
	BOOL useTexture = TRUE;
	float alphaFactor = 1.0f;
	bool rasterizerSolid = true;
	std::string selectedBox = "Basic";

	float outlineScale = 0.1f;
	XMFLOAT3 outlineColor = { 1.0f, 0.6f, 0.1f };
	std::shared_ptr<Fog> fogSystem;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<StencilOutline> stencilOutline;

	Cube cube;
	Quad simpleQuad;
	Sprite crosshair;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;

	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	Camera2D camera2D;
	ImGuiManager imgui;
	std::map<std::string, std::unique_ptr<Camera>> cameras;

	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallNormalTexture;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;
};

#endif