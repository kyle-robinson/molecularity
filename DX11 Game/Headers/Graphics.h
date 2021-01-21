#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics
{
public:
	virtual ~Graphics( void ) = default;
	bool Initialize( HWND hWnd, int width, int height );
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	void Update( float dt );
	UINT GetWidth() const noexcept;
	UINT GetHeight() const noexcept;
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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	UINT windowWidth;
	UINT windowHeight;
};

#endif