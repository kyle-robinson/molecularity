#include "Graphics.h"
#include "Bindables.h"

bool Graphics::Initialize( HWND hWnd, int width, int height )
{
	windowWidth = width;
	windowHeight = height;
	VsynicON = 1;
	AddtoEvent();
	if ( !InitializeDirectX( hWnd ) ) return false;
	if ( !InitializeShaders() ) return false;
	if ( !InitializeRTT() ) return false;

	return true;
}
static HWND* _hWnd;
bool Graphics::InitializeDirectX( HWND hWnd )
{
	try
	{
		_hWnd = &hWnd;
		// pipeline - main components
		swapChain = std::make_shared<Bind::SwapChain>( *this, context.GetAddressOf(), device.GetAddressOf(), hWnd );
		depthStencil = std::make_shared<Bind::DepthStencil>( *this );
		blender = std::make_shared<Bind::Blender>( *this );
		
		// render target - two in use to allow for RTT
		backBuffer = std::make_shared<Bind::RenderTarget>( *this, swapChain->GetSwapChain() );
		renderTarget = std::make_shared<Bind::RenderTarget>( *this );

		// stencils - for outlining models
		stencils.emplace( "Off", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Off ) );
        stencils.emplace( "Mask", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Mask ) );
        stencils.emplace( "Write", std::make_shared<Bind::Stencil>( *this, Bind::Stencil::Mode::Write ) );

		// rasterizers - solid/wireframe && skysphere specific options
		rasterizers.emplace( "Solid", std::make_shared<Bind::Rasterizer>( *this, true, false ) );
        rasterizers.emplace( "Skybox", std::make_shared<Bind::Rasterizer>( *this, true, true ) );
        rasterizers.emplace( "Wireframe", std::make_shared<Bind::Rasterizer>( *this, false, true ) );

		// samplers - point (low quality) && anisotropic (high quality)
        samplers.emplace( "Anisotropic", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Anisotropic ) );
        samplers.emplace( "Bilinear", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Bilinear ) );
        samplers.emplace( "Point", std::make_shared<Bind::Sampler>( *this, Bind::Sampler::Type::Point ) );

		// viewports - main (default camera) && sub (static camera)
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

bool Graphics::InitializeShaders()
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

bool Graphics::InitializeRTT()
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

void Graphics::ClearScene()
{
	// clear render target/depth stencil
	renderTarget->BindAsTexture( *this, depthStencil.get(), clearColor );
    depthStencil->ClearDepthStencil( *this );
}

void Graphics::UpdateRenderState()
{
	samplers[samplerToUse]->Bind( *this );
	stencils["Off"]->Bind( *this );
	blender->Bind( *this );
}

void Graphics::RenderSceneToTexture()
{
	// bind new render target
	backBuffer->BindAsBuffer( *this, clearColor );

	// render fullscreen texture to new render target
	Shaders::BindShaders( context.Get(), vertexShader_full, pixelShader_full );
	fullscreen.SetupBuffers( context.Get(), cb_vs_fullscreen, multiView );
	context->PSSetShaderResources( 0u, 1u, renderTarget->GetShaderResourceViewPtr() );
	Bind::Rasterizer::DrawSolid( *this, fullscreen.ib_full.IndexCount() ); // always draw as solid
}

void Graphics::PresentScene()
{
	// unbind render target
	renderTarget->BindAsNull( *this );
	backBuffer->BindAsNull( *this );

	// display frame
	HRESULT hr = swapChain->GetSwapChain()->Present(VsynicON, NULL );
	if ( FAILED( hr ) )
	{
		hr == DXGI_ERROR_DEVICE_REMOVED ?
            ErrorLogger::Log( device->GetDeviceRemovedReason(), "Swap Chain. Graphics device removed!" ) :
            ErrorLogger::Log( hr, "Swap Chain failed to render frame!" );
		exit( -1 );
	}
}

void Graphics::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
}
#include<JSON_Helper.h>
void Graphics::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::WindowSizeChangeEvent:
	{
		DirectX::XMFLOAT2 _SizeOfScreen = *static_cast<DirectX::XMFLOAT2*>(event->GetData());
		windowWidth = _SizeOfScreen.x;
		windowHeight = _SizeOfScreen.y;
			
	}
	break;
	case EVENTID::UpdateSettingsEvent: 
	{
		//vsysnic
		std::vector<JSON::SettingData> a = *static_cast<std::vector<JSON::SettingData>*>(event->GetData());
		for (auto& setting : a)
		{
			if (setting.Name == "Vsync") {
				VsynicON = std::get<bool>(setting.Setting);
			}
		}
	}
	 break;
	default:
		break;
	}
}
