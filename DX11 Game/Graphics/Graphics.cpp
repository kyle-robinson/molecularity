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
	imgui.Initialize( hWnd, device.Get(), context.Get() );

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

	// Setup Constant Buffers
	cb_ps_light.data.dynamicLightColor = light.lightColor;
	cb_ps_light.data.dynamicLightStrength = light.lightStrength;
	cb_ps_light.data.specularLightColor = light.specularColor;
	cb_ps_light.data.specularLightIntensity = light.specularIntensity;
	cb_ps_light.data.specularLightPower = light.specularPower;

	XMVECTOR lightPosition = camera.GetPositionVector();
	lightPosition += camera.GetForwardVector();
	lightPosition += camera.GetRightVector() / 4;
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
	// Render Games Objects
	/*Shaders::BindShaders( context.Get(), vertexShader_Tex, pixelShader_Tex );
	UINT offset = 0;
	cb_vs_matrix.data.worldMatrix = DirectX::XMMatrixIdentity();
	if ( !cb_vs_matrix.ApplyChanges() ) return;
	context->VSSetConstantBuffers( 0, 1, cb_vs_matrix.GetAddressOf() );
	context->PSSetShaderResources( 0, 1, boxTexture.GetAddressOf() );
	context->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset );
	context->IASetIndexBuffer( indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0 );
	context->DrawIndexed( indexBuffer.IndexCount(), 0, 0 );*/

	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	nanosuit.Draw( camera.GetViewMatrix(), camera.GetProjectionMatrix() );

	//context->PSSetShader( pixelShader_noLight.GetShader(), NULL, 0 );
	light.Draw( camera.GetViewMatrix(), camera.GetProjectionMatrix() );
}

void Graphics::EndFrame()
{
	// Font Rendering
	spriteBatch->Begin();
	static DirectX::XMFLOAT2 fontPosition = { windowWidth - 760.0f, 0.0f };
	spriteFont->DrawString( spriteBatch.get(), L"Font Rendering Demo", fontPosition,
        DirectX::Colors::Black, 0.0f, DirectX::XMFLOAT2( 0.0f, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) );
	spriteBatch->End();

	// Spawn ImGui Windows
	imgui.BeginRender();
	imgui.SpawnDemoWindow();
	imgui.EndRender();

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
	// Update Game Components
	//nanosuit.AdjustRotation( XMFLOAT3( 0.0f, 0.001f * dt, 0.0f ) );
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
		
		context->OMSetRenderTargets( 1, backBuffer.GetAddressOf(), depthStencilView.Get() );

		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = CD3D11_DEPTH_STENCIL_DESC( CD3D11_DEFAULT{} );
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState( &depthStencilStateDesc, depthStencilState.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create depth stencil state!" );

		// Create Viewport
		CD3D11_VIEWPORT viewportDesc = CD3D11_VIEWPORT( 0.0f, 0.0f, windowWidth, windowHeight );
		viewportDesc.MinDepth = 0.0f;
		viewportDesc.MaxDepth = 1.0f;
		context->RSSetViewports( 1u, &viewportDesc );

		// Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC( CD3D11_DEFAULT{} );
		rasterizerDesc.MultisampleEnable = TRUE;
		hr = device->CreateRasterizerState( &rasterizerDesc, &rasterizerState );
		COM_ERROR_IF_FAILED( hr, "Failed to create static rasterizer state!" );
		context->RSSetState( rasterizerState.Get() );

		// Create Sampler State
		CD3D11_SAMPLER_DESC samplerDesc( CD3D11_DEFAULT{} );
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		hr = device->CreateSamplerState( &samplerDesc, samplerState.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create sampler state!" );
		context->PSSetSamplers( 0u, 1u, samplerState.GetAddressOf() );

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
        COM_ERROR_IF_FAILED( hr, "Failed to create texture pixel shader!" );

		// Colour Layout
		D3D11_INPUT_ELEMENT_DESC layoutPosCol[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = vertexShader_Col.Initialize( device, L"Resources\\Shaders\\Primitive_Col.fx", layoutPosCol, ARRAYSIZE( layoutPosCol ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create colour vertex shader!" );
        hr = pixelShader_Col.Initialize( device, L"Resources\\Shaders\\Primitive_Col.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create colour pixel shader!" );*/
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
		/*HRESULT hr = vertexBuffer.Initialize( device.Get(), verticesQuad_Tex, ARRAYSIZE( verticesQuad_Tex ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize triangle vertex buffer!" );
		hr = indexBuffer.Initialize( device.Get(), indicesQuad, ARRAYSIZE( indicesQuad ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize triangle index buffer!" );*/

		nanosuit.SetScale( 1.0f, 1.0f, 1.0f );
		if ( !nanosuit.Initialize( "Resources\\Models\\Nanosuit\\nanosuit.obj", device.Get(), context.Get(), cb_vs_matrix ) )
			return false;

		light.SetScale( 0.01f, 0.01f, 0.01f );
		if ( !light.Initialize( device.Get(), context.Get(), cb_vs_matrix ) )
			return false;

		XMFLOAT2 aspectRatio = { static_cast<float>( windowWidth ), static_cast<float>( windowHeight ) };
		camera.SetPosition( 0.0f, 9.0f, -15.0f );
		camera.SetProjectionValues( 70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f );

		//XMVECTOR lightPosition = camera.GetPositionVector();
		//lightPosition += camera.GetForwardVector() + XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		//light.SetPosition( lightPosition );
		//light.SetRotation( camera.GetRotationFloat3() );

		// Initialize Textures
		HRESULT hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\CrashBox.png", nullptr, boxTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create box texture from file!" );

		// Initialize Constant Buffers
		hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_vs_matrix' Constant Buffer!" );

		hr = cb_ps_light.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'cb_ps_light' Constant Buffer!" );
		cb_ps_light.data.ambientLightColor = XMFLOAT3( 1.0f, 1.0f, 1.0f );
		cb_ps_light.data.ambientLightStrength = 0.1f;
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}