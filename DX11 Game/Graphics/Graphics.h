#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Fog.h"
#include "Cube.h"
#include "Camera.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "JSON_Helper.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
#include "GraphicsContainer.h"
#include "CameraController.h"
#include <dxtk/WICTextureLoader.h>

class StencilOutline;
class TextRenderer;

/// <summary>
/// Loads and renders/updates all the components and models for the current scene/level.
/// Sets up any constant buffers that are specific to this particular scene/level.
/// </summary>
class Graphics : public GraphicsContainer
{
	friend class Application;
public:
	// Functions
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, CameraController* camera, int width, int height );

	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( const float dt );

	// not sure i like using this. Could pass cameras to textRenderer instead of having a passthrough of gets
	CameraController* GetCameraController() const noexcept { return cameras; }
	Cube& GetCube() noexcept { return cube; }

	bool useDefault = true;
	bool useDebug = false;
private:
	bool InitializeScene();
	
	// Variables
	bool useViewportMain;
	bool useViewportSub;

	Cube cube;
	Quad simpleQuad;
	Sprite crosshair;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;

	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	ImGuiManager imgui;
	CameraController* cameras;
	std::shared_ptr<Fog> fogSystem;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<StencilOutline> stencilOutline;

	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallNormalTexture;
	std::map<BoxType, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;
};

#endif