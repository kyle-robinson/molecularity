#pragma once
#ifndef LEVELCONTAINER_H
#define LEVELCONTAINER_H

#include "Graphics.h"
#include "JSON_Helper.h"
#include "MultiViewport.h"

#include "Cube.h"
#include "Camera.h"
#include "Camera2D.h"
#include "CameraController.h"

#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include "Tool_Class.h"
#include "ModelData.h"

class Fog;
class ImGuiManager;
class TextRenderer;
class PostProcessing;
class StencilOutline;
class UI_Manager;

/// <summary>
/// Loads and renders/updates all the components and models for the current scene/level.
/// Sets up any constant buffers that are specific to this particular scene/level.
/// Used to initialize objects native to every level.
/// </summary>
class LevelContainer
{
	friend class Application;
public:
	virtual ~LevelContainer( void ) = default;
	bool Initialize( Graphics* gfx, CameraController* camera, ImGuiManager* imgui, UI_Manager* UI );

	// Render/Update Scene Functions
	void BeginFrame();
	void EndFrame();

	virtual bool OnCreate() = 0;
	virtual void OnSwitch() {}
	virtual void Render() {}
	virtual void RenderFrame();
	virtual void Update( const float dt );
	virtual void CleanUp() {}
	void LateUpdate( const float dt );
	void UpdateCubes( float xPos = -2.5f, float yPos = 0.0f, float zPos = -6.0f, float spacing = 2.5f );

	// not sure i like using this. Could pass cameras to textRenderer instead of having a passthrough of gets
	std::shared_ptr<StencilOutline> GetStencilOutline() const noexcept { return stencilOutline; }
	std::shared_ptr<MultiViewport> GetMultiViewport() const noexcept { return multiViewport; }
	CameraController* GetCameraController() const noexcept { return cameras; }
	std::vector<std::shared_ptr<Cube>>& GetCube() noexcept { return cubes; }
	Graphics* GetGraphics() const noexcept { return graphics; }

	void SetTool( Tool_Class* Tool ) { tool = Tool; }
	Tool_Class* GetTool() { return tool; }

	std::string GetLevelName() { return levelName; }
	int GetNumOfLevelCubes() { return numOfCubes; }

protected:
	void RenderFrameEarly();
	void ShowEndLevelScreen();
	bool levelCompleted = false;

	// Objects
	Tool_Class* tool;
	Graphics* graphics;
	ImGuiManager* imgui;
	CameraController* cameras;
	RenderableGameObject skysphere;
	std::vector<std::shared_ptr<Cube>> cubes;

	// Lights
	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	// Constant Buffers
	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;

	// User Interface
	UI_Manager* _UiManager;

	// Next level data
	std::string levelName;
	UINT32 NextLevel;
	UINT32 CurrentLevel;
	int numOfCubes;

	std::shared_ptr<PostProcessing> postProcessing;
private:
	bool InitializeScene();

	// Systems
	std::shared_ptr<Fog> fogSystem;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<MultiViewport> multiViewport;
	std::shared_ptr<StencilOutline> stencilOutline;

	// Textures
	std::unordered_map<BoxType, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;

	bool isDissCube;
};

#endif