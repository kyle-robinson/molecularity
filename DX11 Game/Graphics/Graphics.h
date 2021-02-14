#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include "Cube.h"
#include "Sprite.h"
#include "Camera.h"
#include "Shaders.h"
#include "Camera2D.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "ImGuiManager.h"
#include "DirectionalLight.h"
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
	
	Cube& GetCube() noexcept { return cube; };
	std::unique_ptr<Camera>& GetCamera( const std::string& cam ) noexcept { return cameras[cam]; };

	// scene parameters
	bool rasterizerSolid = true;
	std::string cameraToUse = "Default";
	std::string samplerToUse = "Anisotropic";

	// cube parameters
	int boxToUse = 0;
	int sizeAmount = 2;
	bool cubeHover = false;
	float sizeToUse = 1.0f;
	std::string selectedBox = "Basic";

	// light parameters
	float useTexture = 1.0f;
	float alphaFactor = 1.0f;

	// outline parameters
	float outlineScale = 0.1f;
	XMFLOAT3 outlineColor = { 1.0f, 0.6f, 0.1f };
private:
	bool InitializeDirectX( HWND hWnd );
	bool InitializeShaders();
	bool InitializeScene();

	void DrawWithOutline( Cube& cube, const XMFLOAT3& color );
	void DrawWithOutline( RenderableGameObject& object, const XMFLOAT3& color );

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spaceTexture;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;

	std::shared_ptr<Bind::Blender> blender;
	std::shared_ptr<Bind::Viewport> viewport;
	std::shared_ptr<Bind::SwapChain> swapChain;
	std::shared_ptr<Bind::RenderTarget> renderTarget;
	std::shared_ptr<Bind::DepthStencil> depthStencil;
	std::map<std::string, std::shared_ptr<Bind::Sampler>> samplers;
	std::map<std::string, std::shared_ptr<Bind::Stencil>> stencils;
	std::map<std::string, std::shared_ptr<Bind::Rasterizer>> rasterizers;

	VertexShader vertexShader_2D;
	VertexShader vertexShader_light;
	VertexShader vertexShader_outline;

	PixelShader pixelShader_2D;
	PixelShader pixelShader_light;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_outline;
	PixelShader pixelShader_2D_discard;

	ConstantBuffer<CB_PS_spot> cb_ps_spot;
	ConstantBuffer<CB_PS_scene> cb_ps_scene;
	ConstantBuffer<CB_PS_point> cb_ps_point;
	ConstantBuffer<CB_VS_matrix> cb_vs_matrix;
	ConstantBuffer<CB_PS_outline> cb_ps_outline;
	ConstantBuffer<CB_VS_matrix_2D> cb_vs_matrix_2d;
	ConstantBuffer<CB_PS_directional> cb_ps_directional;

	UINT windowWidth;
	UINT windowHeight;
	ImGuiManager imgui;

	Sprite crosshair;
	Camera2D camera2D;

	SpotLight spotLight;
	PointLight pointLight;
	DirectionalLight directionalLight;

	Cube cube;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;
	std::map<std::string, std::unique_ptr<Camera>> cameras;

	std::unique_ptr<SpriteFont> spriteFont;
	std::unique_ptr<SpriteBatch> spriteBatch;
};

#endif