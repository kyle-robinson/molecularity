#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <dxtk/WICTextureLoader.h>
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexType.h"
#include "Shaders.h"

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

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> boxTexture;

	VertexBuffer<Vertex_Pos_Tex> vertexBuffer;
	IndexBuffer indexBuffer;

	VertexShader vertexShader_Tex;
	VertexShader vertexShader_Col;
	PixelShader pixelShader_Tex;
	PixelShader pixelShader_Col;

	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;

	UINT windowWidth;
	UINT windowHeight;
};

#endif