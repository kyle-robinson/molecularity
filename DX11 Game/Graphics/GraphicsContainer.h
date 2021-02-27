#pragma once
#ifndef GRAPHICSCONTAINER_H
#define GRAPHICSCONTAINER_H

#include <map>
#include <memory>
#include <string>
#include <Windows.h>
#include <d3d11_1.h>
#include <wrl/client.h>
#include "Shaders.h"
#include "Quad.h"

namespace Bind
{
	class Blender;
	class DepthStencil;
	class Rasterizer;
	class RenderTarget;
	class Sampler;
	class Stencil;
	class SwapChain;
	class Viewport;
}

/// <summary>
/// Initializes DirectX components and shaders required by all scenes/levels.
/// Holds some pipeline functions that clear/present the current frame and bind DirectX components.
/// </summary>
class GraphicsContainer
{
	friend class ImGuiManager;
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
	void RenderSceneToTexture();
	void PresentScene();
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeRTT();
public:
	// Variables
	VertexShader vertexShader_light;
	PixelShader pixelShader_light;
	BOOL multiView = FALSE;
	std::string samplerToUse = "Anisotropic";
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	std::shared_ptr<Bind::RenderTarget> backBuffer;
	std::shared_ptr<Bind::RenderTarget> renderTarget;
	std::map<std::string, std::shared_ptr<Bind::Stencil>> stencils;
	//std::map<std::string, std::shared_ptr<Bind::Viewport>> viewports;
	std::map<std::string, std::shared_ptr<Bind::Rasterizer>> rasterizers;
		
	VertexShader vertexShader_2D;
	VertexShader vertexShader_full;

	PixelShader pixelShader_2D;
	PixelShader pixelShader_full;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_2D_discard;

	BOOL useTexture = TRUE;
	float alphaFactor = 1.0f;
	bool rasterizerSolid = true;
private:
	UINT windowWidth;
	UINT windowHeight;

	QuadFullscreen fullscreen;
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	ConstantBuffer<CB_VS_fullscreen> cb_vs_fullscreen;

	std::shared_ptr<Bind::Blender> blender;
	std::shared_ptr<Bind::Viewport> viewport;
	std::shared_ptr<Bind::SwapChain> swapChain;
	std::shared_ptr<Bind::DepthStencil> depthStencil;
	std::map<std::string, std::shared_ptr<Bind::Sampler>> samplers;
};

#endif