#include "stdafx.h"
#include "Graphics.h"
#include "Vertices.h"
#include "Indices.h"

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
	static float clearColor[4] = { 0.5f, 1.0f, 0.5f, 1.0f };
	context->ClearRenderTargetView( backBuffer.Get(), clearColor );
    context->ClearDepthStencilView( depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	// Set Render State
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	context->OMSetDepthStencilState( depthStencilState.Get(), 0 );
	Shaders::BindShaders( context.Get(), vertexShader_Tex, pixelShader_Tex );
}

void Graphics::RenderFrame()
{
	UINT offset = 0;
	cb_vs_matrix.data.worldMatrix = DirectX::XMMatrixIdentity();
	if ( !cb_vs_matrix.ApplyChanges() ) return;
	context->VSSetConstantBuffers( 0, 1, cb_vs_matrix.GetAddressOf() );
	context->PSSetShaderResources( 0, 1, boxTexture.GetAddressOf() );
	context->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset );
	context->IASetIndexBuffer( indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0 );
	context->DrawIndexed( indexBuffer.IndexCount(), 0, 0 );
}

void Graphics::EndFrame()
{
	// Render UI/ImGui...

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

		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = CD3D11_DEPTH_STENCIL_DESC( CD3D11_DEFAULT{} );
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		hr = device->CreateDepthStencilState( &depthStencilStateDesc, depthStencilState.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil state!" );

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
		// Texture Layout
		D3D11_INPUT_ELEMENT_DESC layoutPosTex[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		HRESULT hr = vertexShader_Tex.Initialize( device, L"Resources\\Shaders\\Primitive_Tex.fx", layoutPosTex, ARRAYSIZE( layoutPosTex ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create vertex shader!" );
        hr = pixelShader_Tex.Initialize( device, L"Resources\\Shaders\\Primitive_Tex.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create pixel shader!" );

		// Colour Layout
		D3D11_INPUT_ELEMENT_DESC layoutPosCol[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = vertexShader_Col.Initialize( device, L"Resources\\Shaders\\Primitive_Col.fx", layoutPosCol, ARRAYSIZE( layoutPosCol ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create vertex shader!" );
        hr = pixelShader_Col.Initialize( device, L"Resources\\Shaders\\Primitive_Col.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create pixel shader!" );
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
		HRESULT hr = vertexBuffer.Initialize( device.Get(), verticesQuad_Tex, ARRAYSIZE( verticesQuad_Tex ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize triangle vertex buffer!" );
		hr = indexBuffer.Initialize( device.Get(), indicesQuad, ARRAYSIZE( indicesQuad ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize triangle index buffer!" );

		// Initialize Texture
		hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\CrashBox.png", nullptr, boxTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create box texture from file!" );

		// Initialize Constant Buffers
		hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_vs_matrix' Constant Buffer!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}