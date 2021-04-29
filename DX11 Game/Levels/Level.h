#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Graphics.h"
#include "JSON_Helper.h"
//#include "ImGuiManager.h"
#include "MultiViewport.h"

#include "Cube.h"
#include "Camera.h"
#include "Camera2D.h"
#include "CameraController.h"

#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include <dxtk/WICTextureLoader.h>

class Fog;
class TextRenderer;
class PostProcessing;
class StencilOutline;

/// <summary>
/// Loads and renders/updates all the components and models for the current scene/level.
/// Sets up any constant buffers that are specific to this particular scene/level.
/// </summary>
class Level : public Graphics
{
	friend class Application;
public:
	virtual ~Level( void ) = default;
	bool Initialize( HWND hWnd, CameraController* camera, int width, int height );

	// Render/Update Scene Functions
	void BeginFrame();
	void EndFrame();

	virtual bool OnCreate() = 0;
	virtual void Render() {}
	virtual void RenderFrame() {}
	virtual void Update( const float dt ) {}

	// not sure i like using this. Could pass cameras to textRenderer instead of having a passthrough of gets
	std::shared_ptr<StencilOutline> GetStencilOutline() const noexcept { return stencilOutline; }
	std::shared_ptr<MultiViewport> GetMultiViewport() const noexcept { return multiViewport; }
	CameraController* GetCameraController() const noexcept { return cameras; }
	std::vector<std::shared_ptr<Cube>>& GetCube() noexcept { return cubes; }
protected:
	//ImGuiManager imgui;

	// Objects
	CameraController* cameras;
	std::vector<std::shared_ptr<Cube>> cubes;

	// Lights
	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	// Constant Buffers
	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;
private:
	bool InitializeScene();

	// Systems
	std::shared_ptr<Fog> fogSystem;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<MultiViewport> multiViewport;
	std::shared_ptr<PostProcessing> postProcessing;
	std::shared_ptr<StencilOutline> stencilOutline;
};

#endif