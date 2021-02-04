#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include "Cube.h"
#include "Light.h"
#include "Camera.h"
#include "Shaders.h"
#include "ImGuiManager.h"
#include "RenderableGameObject.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include <dxtk/WICTextureLoader.h>

namespace Bind
{
	class DepthStencil;
	class Rasterizer;
	class RenderTarget;
	class Sampler;
	class SwapChain;
	class Viewport;
}

class Graphics
{
	friend class GraphicsResource;
public:
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, int width, int height );
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( float dt );
	UINT GetWidth() const noexcept { return windowWidth; }
	UINT GetHeight() const noexcept { return windowHeight; }

	// Global Objects
	Light light;
	RenderableGameObject nanosuit;
	std::unique_ptr<Camera> camera;
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeScene();

	// Device/Context
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> boxTexture;

	// Pipeline State
	std::shared_ptr<Bind::Viewport> viewport;
	std::shared_ptr<Bind::SwapChain> swapChain;
	std::shared_ptr<Bind::RenderTarget> renderTarget;
	std::shared_ptr<Bind::DepthStencil> depthStencil;
	std::map<std::string, std::shared_ptr<Bind::Sampler>> samplers;
	std::map<std::string, std::shared_ptr<Bind::Rasterizer>> rasterizers;

	// Buffers
	VertexBuffer<Vertex_Pos_Tex> vertexBuffer;
	IndexBuffer indexBuffer;

	// Shaders
	VertexShader vertexShader_Tex;
	VertexShader vertexShader_Col;
	VertexShader vertexShader_light;
	PixelShader pixelShader_Tex;
	PixelShader pixelShader_Col;
	PixelShader pixelShader_light;
	PixelShader pixelShader_noLight;

	// Constant Buffers
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_PS_light> cb_ps_light;

	// Local Variables
	UINT windowWidth;
	UINT windowHeight;
	ImGuiManager imgui;
	bool useTexture = true;
	float alphaFactor = 1.0f;
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// Local Objects
	std::unique_ptr<Cube> cube;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
};

#endif