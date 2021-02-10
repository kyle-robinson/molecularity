#include "stdafx.h"
#include "Graphics.h"
#include "Billboard.h"
#include "Stencil.h"
#include "Sampler.h"
#include "Viewport.h"
#include "SwapChain.h"
#include "ModelData.h"
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include <imgui/imgui.h>

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
		D3D11_INPUT_ELEMENT_DESC layoutPosTex[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = vertexShader_Tex.Initialize( device, L"Resources\\Shaders\\Primitive_Tex.fx", layoutPosTex, ARRAYSIZE( layoutPosTex ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create texture vertex shader!" );
        hr = pixelShader_Tex.Initialize( device, L"Resources\\Shaders\\Primitive_Tex.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create texture pixel shader!" );

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
		if ( !ModelData::LoadModelData( "Resources\\Objects.json" ) )
            return false;
        if ( !ModelData::InitializeModelData( context.Get(), device.Get(), cb_vs_matrix, renderables ) )
            return false;

		pointLight.SetScale( 0.01f, 0.01f, 0.01f );
		if ( !pointLight.Initialize( device.Get(), context.Get(), cb_vs_matrix ) )
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
		HRESULT hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\CrashBox.png", nullptr, boxTextures["Default"].GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create BOX texture from file!" );

		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\BounceBox.png", nullptr, boxTextures["Bounce"].GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create BOUNCE BOX texture from file!" );

		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\JumpBox.png", nullptr, boxTextures["Jump"].GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create JUMP CRATE texture from file!" );

		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\TNT.png", nullptr, boxTextures["TNT"].GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create TNT CRATE texture from file!" );
		
		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\Space.png", nullptr, spaceTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create SPACE texture from file!" );

		// Initialize Constant Buffers
		hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_vs_matrix' Constant Buffer!" );

		hr = cb_ps_point.Initialize( device.Get(), context.Get() );
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
	samplers[samplerToUse]->Bind( *this );

	// Render Cubemap First
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_noLight );
	stencils["Off"]->Bind( *this );
	rasterizers["Cubemap"]->Bind( *this );
	skybox->Draw( cb_vs_matrix, spaceTexture.Get() );

	// Reset Rasterizer
	rasterizers[rasterizerSolid ? "Solid" : "Wireframe"]->Bind( *this );

	// Setup Constant Buffers
	pointLight.UpdateConstantBuffer( cb_ps_point, camera );
	cb_ps_point.data.useTexture = useTexture;
	cb_ps_point.data.alphaFactor = alphaFactor;
	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_point.GetAddressOf() );
}

void Graphics::RenderFrame()
{
	// Render Game Objects
	context->PSSetShader( pixelShader_light.GetShader(), NULL, 0 );
	pointLight.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );
	
	// Render List of Models
	for ( auto const& object : renderables )
		renderables[object.first].Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

	// Render Objects w/ Stencils
	if ( cubeHover )
		DrawWithOutline( cube, outlineColor );
	else
		cube->Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );
}

void Graphics::EndFrame()
{
	// Font Rendering
	spriteBatch->Begin();
	if ( toolType == CONVERT )
	{
		spriteFont->DrawString( spriteBatch.get(), L"Multi-Tool: CONVERT",
			DirectX::XMFLOAT2( windowWidth - 760.0f, 0.0f ), DirectX::Colors::White, 0.0f,
			DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) );

		static std::wstring boxType;
		switch ( boxToUse )
		{
		case 0: boxType = L"Default Box"; break;
		case 1: boxType = L"Bounce Box"; break;
		case 2: boxType = L"Jump Box"; break;
		case 3: boxType = L"TNT Box"; break;
		}
		spriteFont->DrawString( spriteBatch.get(), std::wstring( L"Texture: " ).append( boxType ).c_str(),
			DirectX::XMFLOAT2( windowWidth - 260.0f, 0.0f ), DirectX::Colors::Orange, 0.0f,
			DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) );
	}
	else if ( toolType == RESIZE )
	{
		spriteFont->DrawString( spriteBatch.get(), L"Multi-Tool: RESIZE",
			DirectX::XMFLOAT2( windowWidth - 760.0f, 0.0f ), DirectX::Colors::White, 0.0f,
			DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) );

		static std::wstring sizeType;
		switch ( sizeAmount )
		{
		case 0: sizeType = L"Shrink Ray"; break;
		case 1: sizeType = L"Reset Ray"; break;
		case 2: sizeType = L"Growth Ray"; break;
		}
		spriteFont->DrawString( spriteBatch.get(), std::wstring( L"Size: " ).append( sizeType ).c_str(),
			DirectX::XMFLOAT2( windowWidth - 260.0f, 0.0f ), DirectX::Colors::BlueViolet, 0.0f,
			DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) );
	}
	spriteBatch->End();

	// Spawn ImGui Windows
	imgui.BeginRender();
	imgui.SpawnInstructionWindow();
	SpawnControlWindow();
	pointLight.SpawnControlWindow();
	imgui.EndRender();

	// Unbind Render Target
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

	// Set Updated Cube Size
	if ( toolType == RESIZE )
		cube->SetScale( sizeToUse, sizeToUse, sizeToUse );

	// Billboard Model
	float rotation = Billboard::BillboardModel( camera, renderables["Nanosuit"] );
	renderables["Nanosuit"].SetRotation( 0.0f, rotation, 0.0f );
}

//------------------//
// STENCIL OUTLINES //
//------------------//
void Graphics::DrawWithOutline( RenderableGameObject& object, const XMFLOAT3& color )
{
	stencils["Write"]->Bind( *this );
	object.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

	cb_ps_outline.data.outlineColor = color;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1, 1, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	object.SetScale( 1.1f, 1.f, 1.1f );
	object.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_point.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	object.SetScale( 1.0f, 1.0f, 1.0f );
	stencils["Off"]->Bind( *this );
	object.Draw( camera->GetViewMatrix(), camera->GetProjectionMatrix() );
}

void Graphics::DrawWithOutline( std::unique_ptr<Cube>& cube, const XMFLOAT3& color )
{
	stencils["Write"]->Bind( *this );
	cube->Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );

	cb_ps_outline.data.outlineColor = color;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1, 1, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	cube->SetScale( cube->GetScaleFloat3().x + 0.1f, cube->GetScaleFloat3().y + 0.1f, cube->GetScaleFloat3().z + 0.1f );
	cube->Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );

	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_point.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	cube->SetScale( cube->GetScaleFloat3().x - 0.1f, cube->GetScaleFloat3().y - 0.1f, cube->GetScaleFloat3().z - 0.1f );
	stencils["Off"]->Bind( *this );
	cube->Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );
}

//--------------//
// IMGUI WINDOW //
//--------------//
void Graphics::SpawnControlWindow()
{
	if ( ImGui::Begin( "Graphics Controls", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		// Update Texture Usage
		ImGui::Text( "Texture Usage: " );
		ImGui::SameLine();
		static int textureGroup = 0;
		if ( ImGui::RadioButton( "Apply", &textureGroup, 0 ) )
			useTexture = 1.0f;
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Discard", &textureGroup, 1 ) )
			useTexture = 0.0f;
		
		// Update Rasterizer State
		ImGui::Text( "Rasterizer: " );
		ImGui::SameLine();
		static int rasterizerGroup = 0;
	    if ( ImGui::RadioButton( "Solid", &rasterizerGroup, 0 ) )
            rasterizerSolid = true;
	    ImGui::SameLine();
	    if ( ImGui::RadioButton( "Wireframe", &rasterizerGroup, 1 ) )
            rasterizerSolid = false;

		// Update Sampler State
		static int activeSampler = 0;
        static bool selectedSampler[3];
        static std::string previewValueSampler = "Anisotropic";
        static const char* samplerList[]{ "Anisotropic", "Bilinear", "Point Sampling" };
        if ( ImGui::BeginCombo( "Sampler", previewValueSampler.c_str() ) )
        {
            for ( unsigned int i = 0; i < IM_ARRAYSIZE( samplerList ); i++ )
            {
                const bool isSelected = i == activeSampler;
                if ( ImGui::Selectable( samplerList[i], isSelected ) )
                {
                    activeSampler = i;
                    previewValueSampler = samplerList[i];
                }
            }

            switch ( activeSampler )
            {
			case 0: samplerToUse = "Anisotropic"; break;
            case 1: samplerToUse = "Bilinear"; break;
            case 2: samplerToUse = "Point"; break;
            }

            ImGui::EndCombo();
        }

		// Update Stencil Outline Colour
		ImGui::ColorEdit3( "Outline", &outlineColor.x );
	} ImGui::End();
}