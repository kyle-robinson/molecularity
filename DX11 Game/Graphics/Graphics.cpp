#include "stdafx.h"
#include "Graphics.h"
#include "Billboard.h"
#include "Stencil.h"
#include "Sampler.h"
#include "Viewport.h"
#include "SwapChain.h"
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

bool Graphics::Initialize( HWND hWnd, int width, int height )
{
	windowWidth = width;
	windowHeight = height;

	if ( !InitializeDirectX( hWnd ) ) return false;
	if ( !InitializeShaders() ) return false;
	if ( !InitializeScene() ) return false;
	imgui.Initialize( hWnd, device.Get(), context.Get() );

	return true;
}

bool Graphics::InitializeDirectX( HWND hWnd )
{
	try
	{
		swapChain = std::make_shared<Bind::SwapChain>( *this, context.GetAddressOf(), device.GetAddressOf(), hWnd );
		renderTarget = std::make_shared<Bind::RenderTarget>( *this, swapChain->GetSwapChain() );
		depthStencil = std::make_shared<Bind::DepthStencil>( *this );
		viewport = std::make_shared<Bind::Viewport>( *this );

		stencils.emplace( "Off", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Off ) );
        stencils.emplace( "Mask", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Mask ) );
        stencils.emplace( "Write", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Write ) );

		rasterizers.emplace( "Solid", std::make_shared<Bind::Rasterizer>( *this, true, false ) );
        rasterizers.emplace( "Cubemap", std::make_shared<Bind::Rasterizer>( *this, true, true ) );
        rasterizers.emplace( "Wireframe", std::make_shared<Bind::Rasterizer>( *this, false, true ) );

        samplers.emplace( "Anisotropic", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Anisotropic ) );
        samplers.emplace( "Bilinear", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Bilinear ) );
        samplers.emplace( "Point", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Point ) );

		context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// Setup Font Rendering
		spriteBatch = std::make_unique<DirectX::SpriteBatch>( context.Get() );
        spriteFont = std::make_unique<DirectX::SpriteFont>( device.Get(), L"Resources\\Fonts\\open_sans_ms_16.spritefont" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders()
{
	try
	{
		// Normal Layout
		D3D11_INPUT_ELEMENT_DESC layoutPosTexNrm[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		HRESULT hr = vertexShader_light.Initialize( device, L"Resources\\Shaders\\Primitive_Nrm.fx", layoutPosTexNrm, ARRAYSIZE( layoutPosTexNrm ) );
		COM_ERROR_IF_FAILED( hr, "Failed to create light vertex shader!" );
	    hr = pixelShader_light.Initialize( device, L"Resources\\Shaders\\Primitive_Nrm.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create light pixel shader!" );
		hr = pixelShader_noLight.Initialize( device, L"Resources\\Shaders\\Primitive_NoNrm.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create no light pixel shader!" );

		// Texture Layout
		/*D3D11_INPUT_ELEMENT_DESC layoutPosTex[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = vertexShader_Tex.Initialize( device, L"Resources\\Shaders\\Primitive_Tex.fx", layoutPosTex, ARRAYSIZE( layoutPosTex ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create texture vertex shader!" );
        hr = pixelShader_Tex.Initialize( device, L"Resources\\Shaders\\Primitive_Tex.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create texture pixel shader!" );*/

		// Colour Layout
		D3D11_INPUT_ELEMENT_DESC layoutPosCol[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = vertexShader_outline.Initialize( device, L"Resources\\Shaders\\Primitive_Outline.fx", layoutPosCol, ARRAYSIZE( layoutPosCol ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create colour vertex shader!" );
        hr = pixelShader_outline.Initialize( device, L"Resources\\Shaders\\Primitive_Outline.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create colour pixel shader!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		// Initialize Games Objects
		nanosuit.SetScale( 1.0f, 1.0f, 1.0f );
		if ( !nanosuit.Initialize( "Resources\\Models\\Nanosuit\\nanosuit.obj", device.Get(), context.Get(), cb_vs_matrix ) )
			return false;

		light.SetScale( 0.01f, 0.01f, 0.01f );
		if ( !light.Initialize( device.Get(), context.Get(), cb_vs_matrix ) )
			return false;

		cube = std::make_unique<Cube>();
		if ( !cube->Initialize( context.Get(), device.Get() ) )
			return false;
		cube->SetInitialPosition( 0.0f, 5.0f, 5.0f );

		skybox = std::make_unique<Cube>();
		if ( !skybox->Initialize( context.Get(), device.Get() ) )
			return false;
		skybox->SetInitialPosition( 0.0f, 0.0f, 0.0f );
		skybox->SetScale( 250.0f, 250.0f, 250.0f );

		XMFLOAT2 aspectRatio = { static_cast<float>( windowWidth ), static_cast<float>( windowHeight ) };
		camera = std::make_unique<Camera>( 0.0f, 9.0f, -15.0f );
		camera->SetProjectionValues( 70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f );

		// Initialize Textures
		HRESULT hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\CrashBox.png", nullptr, boxTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create BOX texture from file!" );
		boxTextures.push_back( std::move( boxTexture ) );

		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\BounceBox.png", nullptr, bounceBoxTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create BOUNCE BOX texture from file!" );
		boxTextures.push_back( std::move( bounceBoxTexture ) );

		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\JumpBox.png", nullptr, jumpBoxTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create JUMP CRATE texture from file!" );
		boxTextures.push_back( std::move( jumpBoxTexture ) );

		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\TNT.png", nullptr, tntTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create TNT CRATE texture from file!" );
		boxTextures.push_back( std::move( tntTexture ) );
		
		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\Space.png", nullptr, spaceTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create SPACE texture from file!" );

		// Initialize Constant Buffers
		hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_vs_matrix' Constant Buffer!" );

		hr = cb_ps_light.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_ps_light' Constant Buffer!" );

		hr = cb_ps_outline.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_ps_outline' Constant Buffer!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Graphics::BeginFrame()
{
	// Clear Render Target
	renderTarget->BindAsBuffer( *this, depthStencil.get(), clearColor );
    depthStencil->ClearDepthStencil( *this );
	samplers["Anisotropic"]->Bind( *this );

	// Render Cubemap First
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_noLight );
	stencils["Off"]->Bind( *this );
	rasterizers["Cubemap"]->Bind( *this );
	skybox->Draw( cb_vs_matrix, spaceTexture.Get() );

	// Reset Rasterizer
	rasterizers["Solid"]->Bind( *this );

	// Setup Constant Buffers
	cb_ps_light.data.ambientLightColor = light.ambientColor;
	cb_ps_light.data.ambientLightStrength = light.ambientStrength;
	cb_ps_light.data.dynamicLightColor = light.lightColor;
	cb_ps_light.data.dynamicLightStrength = light.lightStrength;
	cb_ps_light.data.specularLightColor = light.specularColor;
	cb_ps_light.data.specularLightStrength = light.specularStrength;
	cb_ps_light.data.specularLightPower = light.specularPower;

	XMVECTOR lightPosition = camera->GetPositionVector();
	lightPosition += camera->GetForwardVector();
	lightPosition += camera->GetRightVector() / 4;
	XMFLOAT3 lightPositionF = XMFLOAT3( XMVectorGetX( lightPosition ), XMVectorGetY( lightPosition ), XMVectorGetZ( lightPosition ) );
	cb_ps_light.data.dynamicLightPosition = lightPositionF;

	cb_ps_light.data.lightConstant = light.constant;
	cb_ps_light.data.lightLinear = light.linear;
	cb_ps_light.data.lightQuadratic = light.quadratic;
	cb_ps_light.data.useTexture = useTexture;
	cb_ps_light.data.alphaFactor = alphaFactor;
	if ( !cb_ps_light.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_light.GetAddressOf() );
}

void Graphics::RenderFrame()
{
	// Render Game Objects
	context->PSSetShader( pixelShader_light.GetShader(), NULL, 0 );
	light.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );
	nanosuit.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

	// Render Objects w/ Stencils
	DrawWithOutline( cube, XMFLOAT3( 1.0f, 0.6f, 0.1f ) );
}

void Graphics::EndFrame()
{
	// Font Rendering
	spriteBatch->Begin();
	static DirectX::XMFLOAT2 fontPosition = { windowWidth - 760.0f, 0.0f };
	spriteFont->DrawString( spriteBatch.get(), L"DirectX 11 Application", fontPosition,
        DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) );
	spriteBatch->End();

	// Spawn ImGui Windows
	imgui.BeginRender();
	light.SpawnControlWindow();
	imgui.EndRender();

	renderTarget->BindAsNull( *this );

	// Display Current Frame
	HRESULT hr = swapChain->GetSwapChain()->Present( 1, NULL );
	if ( FAILED( hr ) )
	{
		hr == DXGI_ERROR_DEVICE_REMOVED ?
            ErrorLogger::Log( device->GetDeviceRemovedReason(), "Swap Chain. Graphics device removed!" ) :
            ErrorLogger::Log( hr, "Swap Chain failed to render frame!" );
		exit( -1 );
	}
}

void Graphics::Update( float dt )
{
	// Update Game Components
	skybox->SetPosition( camera->GetPositionFloat3() );

	// Billboard Model
	float rotation = Billboard::BillboardModel( camera, nanosuit );
	nanosuit.SetRotation( 0.0f, rotation, 0.0f );
}

//------------------//
// STENCIL OUTLINES //
//------------------//
void Graphics::DrawWithOutline( RenderableGameObject& object, const XMFLOAT3& colour )
{
	stencils["Write"]->Bind( *this );
	object.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

	cb_ps_outline.data.outlineColor = colour;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1, 1, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	object.SetScale( 1.1f, 1.f, 1.1f );
	object.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

	if ( !cb_ps_light.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_light.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	object.SetScale( 1.0f, 1.0f, 1.0f );
	stencils["Off"]->Bind( *this );
	object.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );
}

void Graphics::DrawWithOutline( std::unique_ptr<Cube>& cube, const XMFLOAT3& colour )
{
	stencils["Write"]->Bind( *this );
	cube->Draw( cb_vs_matrix, boxTextures[boxToUse].Get() );

	cb_ps_outline.data.outlineColor = colour;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1, 1, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	cube->SetScale( 1.1f, 1.1f, 1.1f );
	cube->Draw( cb_vs_matrix, boxTextures[boxToUse].Get() );

	if ( !cb_ps_light.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_light.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	cube->SetScale( 1.0f, 1.0f, 1.0f );
	stencils["Off"]->Bind( *this );
	cube->Draw( cb_vs_matrix, boxTextures[boxToUse].Get() );
}