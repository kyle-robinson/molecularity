#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include "Cube.h"
#include "Camera.h"
#include "Shaders.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
#include "RenderableGameObject.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include <dxtk/WICTextureLoader.h>

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

class Graphics
{
	friend class GraphicsResource;
public:
	enum ToolType { CONVERT, RESIZE } toolType = CONVERT;
	enum ResizeScale { SMALL, NORMAL, LARGE } resizeScale = LARGE;

	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, int width, int height );
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( float dt );
	UINT GetWidth() const noexcept { return windowWidth; }
	UINT GetHeight() const noexcept { return windowHeight; }

	// Global Objects
	int boxToUse = 0;
	int sizeAmount = 2;
	float sizeToUse = 1.0f;
	bool cubeHover = false;
	
	SpotLight spotLight;
	std::unique_ptr<Cube> cube;
	std::unique_ptr<Camera> camera;
	std::string selectedBox = "Default";
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeScene();
	void SpawnControlWindow();

	void DrawWithOutline( RenderableGameObject& object, const XMFLOAT3& color );
	void DrawWithOutline( std::unique_ptr<Cube>& cube, const XMFLOAT3& color );

	// Device/Context
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	
	// Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spaceTexture;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;

	// Pipeline State
	std::shared_ptr<Bind::Blender> blender;
	std::shared_ptr<Bind::Viewport> viewport;
	std::shared_ptr<Bind::SwapChain> swapChain;
	std::shared_ptr<Bind::RenderTarget> renderTarget;
	std::shared_ptr<Bind::DepthStencil> depthStencil;
	std::map<std::string, std::shared_ptr<Bind::Sampler>> samplers;
	std::map<std::string, std::shared_ptr<Bind::Stencil>> stencils;
	std::map<std::string, std::shared_ptr<Bind::Rasterizer>> rasterizers;

	// Shaders
	VertexShader vertexShader_Tex;
	VertexShader vertexShader_Col;
	VertexShader vertexShader_light;
	VertexShader vertexShader_outline;
	PixelShader pixelShader_Tex;
	PixelShader pixelShader_Col;
	PixelShader pixelShader_light;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_outline;

	// Constant Buffers
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_PS_outline> cb_ps_outline;
	ConstantBuffer<CB_PS_spot> cb_ps_spot;
	ConstantBuffer<CB_PS_point> cb_ps_point;
	ConstantBuffer<CB_PS_directional> cb_ps_directional;

	// Local Variables
	UINT windowWidth;
	UINT windowHeight;
	ImGuiManager imgui;

	bool rasterizerSolid = true;
	std::string samplerToUse = "Anisotropic";

	float useTexture = 1.0f;
	float alphaFactor = 1.0f;
	float outlineScale = 0.1f;
	XMFLOAT3 outlineColor = { 1.0f, 0.6f, 0.1f };
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// Local Objects
	PointLight pointLight;
	DirectionalLight directionalLight;

	std::unique_ptr<DirectX::SpriteFont> spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

	std::unique_ptr<Cube> skybox;
	std::map<std::string, RenderableGameObject> renderables;
};

#endif