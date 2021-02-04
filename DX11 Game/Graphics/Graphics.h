#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Light.h"
#include "Shaders.h"
#include "Camera3D.h"
#include "ImGuiManager.h"
#include "RenderableGameObject.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include <dxtk/WICTextureLoader.h>

class Graphics
{
public:
	Graphics() {}
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, int width, int height );
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( float dt );
	UINT GetWidth() const noexcept { return windowWidth; }
	UINT GetHeight() const noexcept { return windowHeight; }

	Light light;
	Camera3D camera;
	RenderableGameObject nanosuit;
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeScene();

	// Device/Context
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// States
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> boxTexture;

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
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
};

#endif