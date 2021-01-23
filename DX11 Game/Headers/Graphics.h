#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Shaders.h"
#include "VertexType.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include <dxtk/WICTextureLoader.h>

class Graphics
{
public:
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, int width, int height );
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( float dt );
	UINT GetWidth() const noexcept { return windowWidth; }
	UINT GetHeight() const noexcept { return windowHeight; }
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeScene();

	// Device/Context
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	// States
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer;
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
	PixelShader pixelShader_Tex;
	PixelShader pixelShader_Col;

	// Constant Buffers
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;

	// Local Variables
	UINT windowWidth;
	UINT windowHeight;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
};

#endif