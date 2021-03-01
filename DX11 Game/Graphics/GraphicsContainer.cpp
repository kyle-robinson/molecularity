#include "GraphicsContainer.h"
#include "Bindables.h"
#include <imgui/imgui.h>

bool GraphicsContainer::InitializeGraphics( HWND hWnd, int width, int height )
{
	windowWidth = width;
	windowHeight = height;

	if ( !InitializeDirectX( hWnd ) ) return false;
	if ( !InitializeShaders() ) return false;
	if ( !InitializeRTT() ) return false;

	return true;
}

bool GraphicsContainer::InitializeDirectX( HWND hWnd )
{
	try
	{
		swapChain = std::make_shared<Bind::SwapChain>( *this, context.GetAddressOf(), device.GetAddressOf(), hWnd );
		backBuffer = std::make_shared<Bind::RenderTarget>( *this, swapChain->GetSwapChain() );
		renderTarget = std::make_shared<Bind::RenderTarget>( *this );
		depthStencil = std::make_shared<Bind::DepthStencil>( *this );
		blender = std::make_shared<Bind::Blender>( *this );

		stencils.emplace( "Off", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Off ) );
        stencils.emplace( "Mask", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Mask ) );
        stencils.emplace( "Write", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Write ) );

		rasterizers.emplace( "Solid", std::make_shared<Bind::Rasterizer>( *this, true, false ) );
        rasterizers.emplace( "Skybox", std::make_shared<Bind::Rasterizer>( *this, true, true ) );
        rasterizers.emplace( "Wireframe", std::make_shared<Bind::Rasterizer>( *this, false, true ) );

        samplers.emplace( "Anisotropic", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Anisotropic ) );
        samplers.emplace( "Bilinear", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Bilinear ) );
        samplers.emplace( "Point", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Point ) );

		viewports.emplace( "Main", std::make_shared<Bind::Viewport>( *this, Bind::Viewport::Type::Main ) );
		viewports.emplace( "Sub", std::make_shared<Bind::Viewport>( *this, Bind::Viewport::Type::Sub ) );

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
        COM_ERROR_IF_FAILED( hr, "Failed to create 'Sprite' shaders!" );

		// Post-Processing
		hr = vertexShader_full.Initialize( device, L"Resources\\Shaders\\Fullscreen.fx",
			Layout::layoutPos, ARRAYSIZE( Layout::layoutPos ) );
		hr = pixelShader_full.Initialize( device, L"Resources\\Shaders\\Fullscreen.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Fullscreen' shaders!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

bool GraphicsContainer::InitializeRTT()
{
	if ( !fullscreen.Initialize( device.Get() ) ) return false;

	try
	{
		HRESULT hr = cb_vs_fullscreen.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize RTT quad!" );
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
	renderTarget->BindAsTexture( *this, depthStencil.get(), clearColor );
    depthStencil->ClearDepthStencil( *this );
}

void GraphicsContainer::UpdateRenderState()
{
	samplers[samplerToUse]->Bind( *this );
	stencils["Off"]->Bind( *this );
	blender->Bind( *this );
}

void GraphicsContainer::RenderSceneToTexture()
{
	// bind new render target
	backBuffer->BindAsBuffer( *this, clearColor );

	// render fullscreen texture to new render target
	Shaders::BindShaders( context.Get(), vertexShader_full, pixelShader_full );
	fullscreen.SetupBuffers( context.Get(), cb_vs_fullscreen, multiView );
	context->PSSetShaderResources( 0u, 1u, renderTarget->GetShaderResourceViewPtr() );
	Bind::Rasterizer::DrawSolid( *this, fullscreen.ib_full.IndexCount() ); // always draw as solid
}

void GraphicsContainer::PresentScene()
{
	// unbind render target
	renderTarget->BindAsNull( *this );
	backBuffer->BindAsNull( *this );

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