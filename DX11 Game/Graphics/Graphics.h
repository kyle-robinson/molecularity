#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <string>
#include <Windows.h>
#include <d3d11_1.h>
#include <wrl/client.h>
#include <unordered_map>

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
class Graphics
{
	friend class Level1;
	friend class Level2;
	friend class LevelContainer;

	friend class Application;
	friend class ImGuiManager;
	friend class StencilOutline;
	friend class GraphicsResource;
public:
	// Functions
	virtual ~Graphics( void ) = default;
	UINT GetWidth() const noexcept { return windowWidth; }
	UINT GetHeight() const noexcept { return windowHeight; }

	// Pipeline Functions
	bool Initialize( HWND hWnd, int width, int height );
	void ClearScene();
	void UpdateRenderState();
	void RenderSceneToTexture();
	void PresentScene();

	// Pipeline Getters
	std::shared_ptr<Bind::Rasterizer> GetRasterizer( const std::string& rast ) noexcept { return rasterizers[rast]; }
	std::shared_ptr<Bind::Viewport> GetViewport( const std::string& vp ) noexcept { return viewports[vp]; }
	std::shared_ptr<Bind::RenderTarget> GetRenderTarget() noexcept { return renderTarget; }
protected:
	// Device/Context
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// Vertex Shaders
	VertexShader vertexShader_2D;
	VertexShader vertexShader_full;
	VertexShader vertexShader_light;

	// Pixel Shaders
	PixelShader pixelShader_2D;
	PixelShader pixelShader_full;
	PixelShader pixelShader_light;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_2D_discard;

	// Constant Buffer Variables
	BOOL multiView = FALSE;
	BOOL useTexture = TRUE;
	float alphaFactor = 1.0f;
	bool rasterizerSolid = true;
	std::string samplerToUse = "Anisotropic";
private:
	// Initialization Functions
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeRTT();

	UINT windowWidth;
	UINT windowHeight;

	// Viewport/RTT Components
	QuadFullscreen fullscreen;
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	ConstantBuffer<CB_VS_fullscreen> cb_vs_fullscreen;

	// DirectX Pipeline Components
	std::shared_ptr<Bind::Blender> blender;
	std::shared_ptr<Bind::SwapChain> swapChain;
	std::shared_ptr<Bind::RenderTarget> backBuffer;
	std::shared_ptr<Bind::RenderTarget> renderTarget;
	std::shared_ptr<Bind::DepthStencil> depthStencil;
	std::unordered_map<std::string, std::shared_ptr<Bind::Stencil>> stencils;
	std::unordered_map<std::string, std::shared_ptr<Bind::Sampler>> samplers;
	std::unordered_map<std::string, std::shared_ptr<Bind::Viewport>> viewports;
	std::unordered_map<std::string, std::shared_ptr<Bind::Rasterizer>> rasterizers;
};

#endif