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
		//   MODELS   //
		HRESULT hr = vertexShader_light.Initialize( device, L"Resources\\Shaders\\Model_Nrm.fx",
			Layout::layoutPosTexNrm, ARRAYSIZE( Layout::layoutPosTexNrm ) );
	    hr = pixelShader_light.Initialize( device, L"Resources\\Shaders\\Model_Nrm.fx" );
		hr = pixelShader_noLight.Initialize( device, L"Resources\\Shaders\\Model_NoNrm.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Light' shaders!" );

		//   SPRITES   //
		hr = vertexShader_2D.Initialize( device, L"Resources\\Shaders\\Sprite.fx",
			Layout::layoutPosTex, ARRAYSIZE( Layout::layoutPosTex ) );
		hr = pixelShader_2D.Initialize( device, L"Resources\\Shaders\\Sprite.fx" );
		hr = pixelShader_2D_discard.Initialize( device, L"Resources\\Shaders\\Sprite_Discard.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create sprite 'Sprite' shaders!" );

		//   OUTLINE   //
		hr = vertexShader_outline.Initialize( device, L"Resources\\Shaders\\Model_Outline.fx",
			Layout::layoutPosCol, ARRAYSIZE( Layout::layoutPosCol ) );
		hr = pixelShader_outline.Initialize( device, L"Resources\\Shaders\\Model_Outline.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create 'Colour' shaders!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

// RENDER STATE //
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

// TEXT RENDERING //
void GraphicsContainer::RenderSceneText()
{
	if ( cubeInRange && cubeHover && !holdingCube )
	{
		textRenderer->DrawString( L"Press 'E' to pick up cube.",
			XMFLOAT2( windowWidth / 2 - 120.0f, windowHeight / 2 - 40.0f ), Colors::LightGreen );
	}
	if ( toolType == CONVERT )
	{
		textRenderer->DrawString( L"Multi-Tool: CONVERT",
			XMFLOAT2( windowWidth - 760.0f, 0.0f ), Colors::White );

		static std::wstring boxType;
		switch ( boxToUse )
		{
		case 0: boxType = L"Default Box"; break;
		case 1: boxType = L"Bounce Box"; break;
		case 2: boxType = L"Jump Box"; break;
		case 3: boxType = L"TNT Box"; break;
		}

		textRenderer->DrawString( std::wstring( L"Texture: " ).append( boxType ).c_str(),
			XMFLOAT2( windowWidth - 260.0f, 0.0f ), Colors::Orange );
	}
	else if ( toolType == RESIZE )
	{
		textRenderer->DrawString( L"Multi-Tool: RESIZE",
			XMFLOAT2( windowWidth - 760.0f, 0.0f ), Colors::White );

		static std::wstring sizeType;
		switch ( sizeAmount )
		{
		case 0: sizeType = L"Shrink Ray"; break;
		case 1: sizeType = L"Reset Ray"; break;
		case 2: sizeType = L"Growth Ray"; break;
		}

		textRenderer->DrawString( std::wstring( L"Size: " ).append( sizeType ).c_str(),
			XMFLOAT2( windowWidth - 260.0f, 0.0f ), Colors::BlueViolet );
	}
	textRenderer->DrawString( std::wstring( L"Camera: " ).append( StringConverter::StringToWide( cameraToUse ) ).c_str(),
		XMFLOAT2( 20.0f, 0.0f ), Colors::IndianRed );
}

// STENCIL OUTLINES //
void GraphicsContainer::DrawWithOutline( Cube& cube, float scale, XMFLOAT3& color, ID3D11ShaderResourceView* texture )
{
	// write pixels to the buffer, which will act as the stencil mask
	stencils["Write"]->Bind( *this );
	cube.Draw( cb_vs_matrix, texture );

	// scale the cube and draw the stencil outline, ignoring the pixels previously written to the buffer
	cb_ps_outline.data.outlineColor = color;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	cube.SetScale( cube.GetScaleFloat3().x + scale,
		cube.GetScaleFloat3().y + scale, cube.GetScaleFloat3().z + scale );
	cube.Draw( cb_vs_matrix, texture );

	// rescale the cube and draw using the appropriate shaders and textures
	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	cube.SetScale( cube.GetScaleFloat3().x - scale,
		cube.GetScaleFloat3().y - scale, cube.GetScaleFloat3().z - scale );
	stencils["Off"]->Bind( *this );
	cube.Draw( cb_vs_matrix, texture );
}

void GraphicsContainer::DrawWithOutline( RenderableGameObject& object, float scale, XMFLOAT3& color )
{
	// write pixels to the buffer, which will act as the stencil mask
	stencils["Write"]->Bind( *this );
	object.Draw();

	// scale the model and draw the stencil outline, ignoring the pixels previously written to the buffer
	cb_ps_outline.data.outlineColor = color;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	object.SetScale( object.GetScaleFloat3().x + scale, 1.0f, object.GetScaleFloat3().z + scale );
	object.Draw();

	// rescale the model and draw using the appropriate shaders and textures
	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	object.SetScale( object.GetScaleFloat3().x - scale, 1.0f, object.GetScaleFloat3().z - scale );
	stencils["Off"]->Bind( *this );
	object.Draw();
}

// IMGUI WINDOWS //
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

// PRESENT FRAME //
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