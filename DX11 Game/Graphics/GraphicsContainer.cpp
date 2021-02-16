#include "stdafx.h"
#include "GraphicsContainer.h"
#include "Bindables.h"
#include <imgui/imgui.h>

bool GraphicsContainer::InitializeGraphics( HWND hWnd, int width, int height )
{
	windowWidth = width;
	windowHeight = height;

	if ( !InitializeDirectX( hWnd ) ) return false;
	if ( !InitializeShaders() ) return false;
	imgui.Initialize( hWnd, device.Get(), context.Get() );

	return true;
}

bool GraphicsContainer::InitializeDirectX( HWND hWnd )
{
	try
	{
		swapChain = std::make_shared<Bind::SwapChain>( *this, context.GetAddressOf(), device.GetAddressOf(), hWnd );
		renderTarget = std::make_shared<Bind::RenderTarget>( *this, swapChain->GetSwapChain() );
		depthStencil = std::make_shared<Bind::DepthStencil>( *this );
		viewport = std::make_shared<Bind::Viewport>( *this );
		blender = std::make_shared<Bind::Blender>( *this );

		stencils.emplace( "Off", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Off ) );
        stencils.emplace( "Mask", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Mask ) );
        stencils.emplace( "Write", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Write ) );
		stencilOutline = std::make_shared<Bind::StencilOutline>( *this, outlineScale, outlineColor );

		rasterizers.emplace( "Solid", std::make_shared<Bind::Rasterizer>( *this, true, false ) );
        rasterizers.emplace( "Skybox", std::make_shared<Bind::Rasterizer>( *this, true, true ) );
        rasterizers.emplace( "Wireframe", std::make_shared<Bind::Rasterizer>( *this, false, true ) );

        samplers.emplace( "Anisotropic", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Anisotropic ) );
        samplers.emplace( "Bilinear", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Bilinear ) );
        samplers.emplace( "Point", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Point ) );

		textRenderer = std::make_shared<Bind::TextRenderer>( *this, L"open_sans_ms_16.spritefont" );
		context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

bool GraphicsContainer::InitializeShaders()
{
	try
	{
		// Models
		HRESULT hr = vertexShader_light.Initialize( device, L"Resources\\Shaders\\Model_Nrm.fx",
			Layout::layoutPosTexNrm, ARRAYSIZE( Layout::layoutPosTexNrm ) );
	    hr = pixelShader_light.Initialize( device, L"Resources\\Shaders\\Model_Nrm.fx" );
		hr = pixelShader_noLight.Initialize( device, L"Resources\\Shaders\\Model_NoNrm.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Light' shaders!" );

		// Sprites
		hr = vertexShader_2D.Initialize( device, L"Resources\\Shaders\\Sprite.fx",
			Layout::layoutPosTex, ARRAYSIZE( Layout::layoutPosTex ) );
		hr = pixelShader_2D.Initialize( device, L"Resources\\Shaders\\Sprite.fx" );
		hr = pixelShader_2D_discard.Initialize( device, L"Resources\\Shaders\\Sprite_Discard.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create sprite 'Sprite' shaders!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void GraphicsContainer::ClearScene()
{
	// clear render target/depth stencil
	static float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	renderTarget->BindAsBuffer( *this, depthStencil.get(), clearColor );
    depthStencil->ClearDepthStencil( *this );
}

void GraphicsContainer::UpdateRenderState()
{
	samplers[samplerToUse]->Bind( *this );
	stencils["Off"]->Bind( *this );
	blender->Bind( *this );
}

void GraphicsContainer::RenderSceneText()
{
	textRenderer->RenderCubeMoveText( *this );
	textRenderer->RenderMultiToolText( *this );
	textRenderer->RenderCameraText( *this );
}

void GraphicsContainer::RenderImGuiWindows()
{
	if ( cameraToUse == "Debug" )
	{
		imgui.BeginRender();
		imgui.SpawnInstructionWindow();
		imgui.SpawnGraphicsWindow( *this );
		pointLight.SpawnControlWindow();
		directionalLight.SpawnControlWindow();
		spotLight.SpawnControlWindow();
		imgui.EndRender();
	}
}

void GraphicsContainer::PresentScene()
{
	// unbind render target
	renderTarget->BindAsNull( *this );

	// display frame
	HRESULT hr = swapChain->GetSwapChain()->Present( 1, NULL );
	if ( FAILED( hr ) )
	{
		hr == DXGI_ERROR_DEVICE_REMOVED ?
            ErrorLogger::Log( device->GetDeviceRemovedReason(), "Swap Chain. Graphics device removed!" ) :
            ErrorLogger::Log( hr, "Swap Chain failed to render frame!" );
		exit( -1 );
	}
}