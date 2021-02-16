#pragma once
#ifndef GRAPHICSCONTAINER_H
#define GRAPHICSCONTAINER_H

#include <map>
#include "Fog.h"
#include "Camera.h"
#include "Shaders.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"

namespace Bind
{
	class Blender;
	class DepthStencil;
	class Fog;
	class Rasterizer;
	class RenderTarget;
	class Sampler;
	class Stencil;
	class StencilOutline;
	class SwapChain;
	class TextRenderer;
	class Viewport;
}

class GraphicsContainer
{
	friend class GraphicsResource;
public:
	// Functions
	virtual ~GraphicsContainer( void ) = default;
	UINT GetWidth() const noexcept { return windowWidth; }
	UINT GetHeight() const noexcept { return windowHeight; }
protected:
	bool InitializeGraphics( HWND hWnd, int width, int height );
	void ClearScene();
	void UpdateRenderState();

	void RenderSceneText();
	void RenderImGuiWindows();
	void PresentScene();
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
public:
	// Variables
	int boxToUse = 0;
	int sizeAmount = 2;
	float sizeToUse = 1.0f;
	bool cubeHover = false;
	float useTexture = 1.0f;
	float alphaFactor = 1.0f;
	bool cubeInRange = false;
	bool holdingCube = false;

	float outlineScale = 0.1f;
	bool rasterizerSolid = true;
	std::string selectedBox = "Basic";
	std::string cameraToUse = "Default";
	std::string samplerToUse = "Anisotropic";
	XMFLOAT3 outlineColor = { 1.0f, 0.6f, 0.1f };
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	std::shared_ptr<Bind::TextRenderer> textRenderer;
	std::shared_ptr<Bind::StencilOutline> stencilOutline;
	std::map<std::string, std::shared_ptr<Bind::Rasterizer>> rasterizers;

	Fog fog;
	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;
	std::map<std::string, std::unique_ptr<Camera>> cameras;

	VertexShader vertexShader_2D;
	VertexShader vertexShader_light;
	PixelShader pixelShader_2D;
	PixelShader pixelShader_light;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_2D_discard;

	ConstantBuffer<CB_PS_point> cb_ps_point;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
private:
	UINT windowWidth;
	UINT windowHeight;
	ImGuiManager imgui;

	std::shared_ptr<Bind::Blender> blender;
	std::shared_ptr<Bind::Viewport> viewport;
	std::shared_ptr<Bind::SwapChain> swapChain;
	std::shared_ptr<Bind::RenderTarget> renderTarget;
	std::shared_ptr<Bind::DepthStencil> depthStencil;
	std::map<std::string, std::shared_ptr<Bind::Sampler>> samplers;
	std::map<std::string, std::shared_ptr<Bind::Stencil>> stencils;
};

#endif