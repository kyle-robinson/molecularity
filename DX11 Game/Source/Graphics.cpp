#include "stdafx.h"
#include "Graphics.h"

bool Graphics::Initialize( HWND hWnd, int width, int height )
{
	windowWidth = width;
	windowHeight = height;

	if ( !InitializeDirectX( hWnd ) ) return false;
	if ( !InitializeShaders() ) return false;
	if ( !InitializeScene() ) return false;

	return true;
}

void Graphics::BeginFrame()
{
	// Clear Render Target
	static float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	context->ClearRenderTargetView( backBuffer.Get(), clearColor );
    context->ClearDepthStencilView( depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	// Set Render State
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Graphics::RenderFrame()
{
	// Render Game Components...
}

void Graphics::EndFrame()
{
	// Display Current Frame
	HRESULT hr = swapChain->Present( 1, NULL );
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
	// Update Game Components...
}

UINT Graphics::GetWidth() const noexcept
{
	return windowWidth;
}

UINT Graphics::GetHeight() const noexcept
{
	return windowHeight;
}

bool Graphics::InitializeDirectX( HWND hWnd )
{
	try
	{
		UINT createDeviceFlags = 0;
    #ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

		// Create Device & Swap Chain
        DXGI_SWAP_CHAIN_DESC sd = { 0 };
        sd.BufferCount = 1;
        sd.BufferDesc.Width = static_cast<FLOAT>( windowWidth );
        sd.BufferDesc.Height = static_cast<FLOAT>( windowHeight );
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hWnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,                    // IDXGI Adapter
            D3D_DRIVER_TYPE_HARDWARE,   // Driver Type
            nullptr,                    // Software Module
            createDeviceFlags,          // Flags for Runtime Layers
            nullptr,                    // Feature Levels Array
            0,                          // No. of Feature Levels
            D3D11_SDK_VERSION,          // SDK Version
            &sd,                        // Swap Chain Description
            swapChain.GetAddressOf(),   // Swap Chain Address
            device.GetAddressOf(),      // Device Address
            nullptr,                    // Ptr to Feature Level
            context.GetAddressOf()      // Context Address
        );
        COM_ERROR_IF_FAILED( hr, "Failed to create Device and Swap Chain!" );

		// Create Back Buffer
        Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
		hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )pBackBuffer.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create Back Buffer!" );
		hr = device->CreateRenderTargetView( pBackBuffer.Get(), nullptr, backBuffer.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create Render Target View with Back Buffer!" );
		context->OMSetRenderTargets( 1, backBuffer.GetAddressOf(), nullptr );

		// Create Depth Stencil State
		CD3D11_TEXTURE2D_DESC depthStencilDesc( DXGI_FORMAT_D24_UNORM_S8_UINT, windowWidth, windowHeight );
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		hr = device->CreateTexture2D( &depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil texture!" );
		hr = device->CreateDepthStencilView( depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil view!" );

		// Create Viewport
		CD3D11_VIEWPORT viewportDesc = CD3D11_VIEWPORT( 0.0f, 0.0f, windowWidth, windowHeight );
		context->RSSetViewports( 1u, &viewportDesc );

		// Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC( CD3D11_DEFAULT{} );
		rasterizerDesc.MultisampleEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		hr = device->CreateRasterizerState( &rasterizerDesc, &pRasterizer );
		COM_ERROR_IF_FAILED( hr, "Failed to create static rasterizer state!" );
		context->RSSetState( pRasterizer.Get() );

		// Create Sampler State
		CD3D11_SAMPLER_DESC samplerDesc( CD3D11_DEFAULT{} );
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		hr = device->CreateSamplerState( &samplerDesc, pSampler.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create sampler state!" );
		context->PSSetSamplers( 0u, 1u, pSampler.GetAddressOf() );
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
		// Initialize Shaders...
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
		// Initialize Game Objects...
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}