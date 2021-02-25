#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

/// <summary>
/// Loads and renders/updates all the components and models for the current scene/level.
/// Sets up any constant buffers that are specific to this particular scene/level.
/// </summary>

#include "Fog.h"
#include "Cube.h"
#include "Quad.h"
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

// Will need moved when cameraClass is made for itself or even moved to 
//static enum class CameraTypes { Default, Static, Debug };

class Graphics : public GraphicsContainer
{
	friend class Application;
public:
	//might move this. It doenst do anything at the moment so left it incase.
	enum ResizeScale { SMALL, NORMAL, LARGE } resizeScale = LARGE;

	// Functions
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd,CameraController* camera ,int width, int height );

	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( const float dt );
	
	Cube& GetCube() noexcept { return cube; }

	CameraController* GetCameraController() { return cameras; } //not sure i like using this. Could pass cameras to textRenderer instead of having a passthrough of gets

private:
	bool InitializeScene();

public:
	// Variables - to be moved to a cameraController (and changed by input class)
	int boxToUse = 0;
	int sizeAmount = 2;
	bool cubeHover = false;
	bool cubeInRange = false;

	std::string selectedBox = "Basic"; // box needs to know what type of box it is, this will need to be moved possibly
	float sizeToUse = 1.0f; // box should know what size it will be
private:
	Cube cube;
	Quad simpleQuad;
	Sprite crosshair;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;

	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	ImGuiManager imgui;
	std::shared_ptr<Fog> fogSystem;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<StencilOutline> stencilOutline;

	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickwallNormalTexture;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;

	CameraController* cameras;
};

#endif