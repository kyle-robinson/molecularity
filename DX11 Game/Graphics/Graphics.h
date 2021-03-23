#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Cube.h"
#include "Camera.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "JSON_Helper.h"
#include "ImGuiManager.h"
#include "MultiViewport.h"
#include "CameraController.h"
#include "DirectionalLight.h"
#include "GraphicsContainer.h"
#include"UI.h"
#include <dxtk/WICTextureLoader.h>

class Fog;
class TextRenderer;
class PostProcessing;
class StencilOutline;

/// <summary>
/// Loads and renders/updates all the components and models for the current scene/level.
/// Sets up any constant buffers that are specific to this particular scene/level.
/// </summary>
class Graphics : public GraphicsContainer
{
	friend class Application;
public:
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, CameraController* camera, int width, int height );

	// Render/Update Scene Functions
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( const float dt );

	// not sure i like using this. Could pass cameras to textRenderer instead of having a passthrough of gets
	std::shared_ptr<MultiViewport> GetMultiViewport() const noexcept { return multiViewport; }
	CameraController* GetCameraController() const noexcept { return cameras; }
	std::vector<std::shared_ptr<Cube>>& GetCube() noexcept { return cubes; }

	UI GetUi() { return UI_Controllor; }

private:
	bool InitializeScene();
	UI UI_Controllor;
	// Scene Objects
	Quad simpleQuad;
	Sprite crosshair;
	CameraController* cameras;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;
	RenderableGameObject pressurePlate;
	std::vector<std::shared_ptr<Cube>> cubes;

	// Lights
	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	// Systems
	ImGuiManager imgui;
	std::shared_ptr<Fog> fogSystem;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<MultiViewport> multiViewport;
	std::shared_ptr<PostProcessing> postProcessing;
	std::shared_ptr<StencilOutline> stencilOutline;

	// Constant Buffers
	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;

	// Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallNormalTexture;
	std::map<BoxType, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;
};

#endif