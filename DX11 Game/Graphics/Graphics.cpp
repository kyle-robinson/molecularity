#include "Graphics.h"
#include "Bindables.h"
#include "Billboard.h"
#include "ModelData.h"
#include "Collisions.h"
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

bool Graphics::InitializeShaders()
{
	try
	{
		/*   MODELS   */
		HRESULT hr = vertexShader_light.Initialize( device, L"Resources\\Shaders\\Model_Nrm.fx",
			Layout::layoutPosTexNrm, ARRAYSIZE( Layout::layoutPosTexNrm ) );
	    hr = pixelShader_light.Initialize( device, L"Resources\\Shaders\\Model_Nrm.fx" );
		hr = pixelShader_noLight.Initialize( device, L"Resources\\Shaders\\Model_NoNrm.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Light' shaders!" );

		/*   SPRITES   */
		hr = vertexShader_2D.Initialize( device, L"Resources\\Shaders\\Sprite.fx",
			Layout::layoutPosTex, ARRAYSIZE( Layout::layoutPosTex ) );
		hr = pixelShader_2D.Initialize( device, L"Resources\\Shaders\\Sprite.fx" );
		hr = pixelShader_2D_discard.Initialize( device, L"Resources\\Shaders\\Sprite_Discard.fx" );
        COM_ERROR_IF_FAILED( hr, "Failed to create sprite 'Sprite' shaders!" );

		/*   OUTLINE   */
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

bool Graphics::InitializeScene()
{
	try
	{
		/*   MODELS   */
		if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );

		if ( !skysphere.Initialize( "Resources\\Models\\Sphere\\sphere.obj", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		skysphere.SetInitialScale( 250.0f, 250.0f, 250.0f );

		/*   LIGHTS   */
		if ( !pointLight.Initialize( "Resources\\Models\\Disco\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		pointLight.SetInitialPosition( -5.0f, 9.0f, -10.0f );
		pointLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

		if ( !directionalLight.Initialize( "Resources\\Models\\Disco\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		directionalLight.SetInitialPosition( 10.0f, 20.0f, 10.0f );
		directionalLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

		if ( !spotLight.Initialize( "Resources\\Models\\Flashlight.fbx", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		spotLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

		/*   PRIMITIVES   */
		if ( !cube.Initialize( context.Get(), device.Get() ) ) return false;
		cube.SetInitialPosition( 0.0f, 5.0f, 5.0f );

		if ( !simpleQuad.Initialize( context.Get(), device.Get() ) ) return false;
		simpleQuad.SetInitialPosition( 0.0f, 5.0f, -5.0f );
		simpleQuad.SetInitialRotation( simpleQuad.GetRotationFloat3().x + XM_PI, simpleQuad.GetRotationFloat3().y + XM_PI, simpleQuad.GetRotationFloat3().z );

		/*   SPRITES   */
		if ( !crosshair.Initialize( device.Get(), context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
        crosshair.SetInitialPosition( windowWidth / 2 - crosshair.GetWidth() / 2, windowHeight / 2 - crosshair.GetHeight() / 2, 0 );

		/*   CAMERAS   */
		XMFLOAT2 aspectRatio = { static_cast<float>( windowWidth ), static_cast<float>( windowHeight ) };
		cameras.emplace( "Default", std::make_unique<Camera>( 0.0f, 9.0f, -15.0f ) );
		cameras.emplace( "Static", std::make_unique<Camera>( 0.0f, 9.0f, 0.0f ) );
		cameras.emplace( "Debug", std::make_unique<Camera>( 0.0f, 9.0f, -10.0f ) );
		for ( const auto& cam : cameras )
			cam.second->SetProjectionValues( 70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f );
		camera2D.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );

		/*   TEXTURES   */
		HRESULT hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\basic_crate.png", nullptr, boxTextures["Basic"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\bounce_crate.png", nullptr, boxTextures["Bounce"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\arrow_crate.png", nullptr, boxTextures["Arrow"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\tnt_crate.png", nullptr, boxTextures["TNT"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\brickwall.jpg", nullptr, brickwallTexture.GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\brickwall_normal.jpg", nullptr, brickwallNormalTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create texture from file!" );

		/*   CONSTANT BUFFERS   */
		hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
		hr = cb_ps_outline.Initialize( device.Get(), context.Get() );
		hr = cb_ps_scene.Initialize( device.Get(), context.Get() );
		hr = cb_ps_point.Initialize( device.Get(), context.Get() );
		hr = cb_ps_directional.Initialize( device.Get(), context.Get() );
		hr = cb_ps_spot.Initialize( device.Get(), context.Get() );
		hr = cb_vs_matrix_2d.Initialize( device.Get(), context.Get() );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize constant buffer!" );
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
	// clear render target
	static float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	renderTarget->BindAsBuffer( *this, depthStencil.get(), clearColor );
    depthStencil->ClearDepthStencil( *this );

	// set render state
	samplers[samplerToUse]->Bind( *this );
	stencils["Off"]->Bind( *this );
	blender->Bind( *this );

	// render skysphere first
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_noLight );
	rasterizers["Skybox"]->Bind( *this );
	skysphere.Draw( cameras[cameraToUse] );
	rasterizers[rasterizerSolid ? "Solid" : "Wireframe"]->Bind( *this );

	// update constant buffers
	cb_ps_scene.data.useTexture = useTexture;
	cb_ps_scene.data.alphaFactor = alphaFactor;
	cb_ps_scene.data.useNormalMap = 0.0f;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );

	pointLight.UpdateConstantBuffer( cb_ps_point, cameras[cameraToUse] );
	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );

	directionalLight.UpdateConstantBuffer( cb_ps_directional );
	if ( !cb_ps_directional.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 4u, 1u, cb_ps_directional.GetAddressOf() );

	spotLight.UpdateConstantBuffer( cb_ps_spot, cameras["Default"] );
	if ( !cb_ps_spot.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 5u, 1u, cb_ps_spot.GetAddressOf() );
}

void Graphics::RenderFrame()
{
	// render models w/out normals
	pointLight.Draw( cameras[cameraToUse] );
	directionalLight.Draw( cameras[cameraToUse] );
	
	// render models w/ normals
	context->PSSetShader( pixelShader_light.GetShader(), NULL, 0 );
	spotLight.Draw( cameras[cameraToUse] );
	hubRoom.Draw( cameras[cameraToUse] );

	// render models w/ normal maps
	cb_ps_scene.data.useNormalMap = 1.0f;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );
	simpleQuad.Draw( cb_vs_matrix, brickwallTexture.Get(), brickwallNormalTexture.Get() );

	cb_ps_scene.data.useNormalMap = 0.0f;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );

	// render objects w/ stencils
	cubeHover ? DrawWithOutline( cube, outlineColor ) :
		cube.Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );

	// render sprites
	Shaders::BindShaders( context.Get(), vertexShader_2D, pixelShader_2D );
	cb_ps_scene.data.useTexture = true;
    if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
    crosshair.Draw( camera2D.GetWorldOrthoMatrix() );
}

void Graphics::EndFrame()
{
	RenderSceneText();

	// display imgui
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

void Graphics::Update( float dt )
{
	UNREFERENCED_PARAMETER( dt );

	// update lights/skysphere
	pointLight.SetPosition( pointLight.GetLightPosition() );
	directionalLight.SetPosition( directionalLight.GetLightPosition() );
	skysphere.SetPosition( cameras[cameraToUse]->GetPositionFloat3() );

	// camera world collisions
	bool wallCollision = Collisions::CheckCollisionCircle( cameras["Default"], hubRoom, 25.0f );
	if ( !wallCollision )
	{
		float dx = hubRoom.GetPositionFloat3().x - cameras["Default"]->GetPositionFloat3().x;
		float dz = hubRoom.GetPositionFloat3().z - cameras["Default"]->GetPositionFloat3().z;
		float length = std::sqrtf( dx * dx + dz * dz );
		dx /= length;
		dz /= length;
		dx *= cameras["Default"]->GetCameraSpeed() * 10.0f;
		dz *= cameras["Default"]->GetCameraSpeed() * 10.0f;
		cameras["Default"]->AdjustPosition( dx, 0.0f, dz );
	}

	// prevent camera y-axis movement
	cameras["Default"]->SetPosition(
		cameras["Default"]->GetPositionFloat3().x,
		9.0f,
		cameras["Default"]->GetPositionFloat3().z
	);
	
	// cube range collision check
	if ( toolType == RESIZE )cube.SetScale( sizeToUse, sizeToUse, sizeToUse );
	cubeInRange = Collisions::CheckCollisionSphere( cameras[cameraToUse], cube, 5.0f );

	// set position of spot light model
	XMVECTOR spotLightPosition = cameras["Default"]->GetPositionVector();
	spotLightPosition += cameras["Default"]->GetForwardVector() / 4;
	spotLightPosition += cameras["Default"]->GetRightVector() / 2;
	spotLight.SetPosition( spotLightPosition );
	spotLight.SetRotation(
		cameras["Default"]->GetRotationFloat3().x + XM_PI,
		cameras["Default"]->GetRotationFloat3().y,
		cameras["Default"]->GetRotationFloat3().z
	);
}

// TEXT RENDERING //
void Graphics::RenderSceneText()
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
void Graphics::DrawWithOutline( Cube& cube, const XMFLOAT3& color )
{
	// write pixels to the buffer, which will act as the stencil mask
	stencils["Write"]->Bind( *this );
	cube.Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );

	// scale the cube and draw the stencil outline, ignoring the pixels previously written to the buffer
	cb_ps_outline.data.outlineColor = color;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	cube.SetScale( cube.GetScaleFloat3().x + outlineScale,
		cube.GetScaleFloat3().y + outlineScale, cube.GetScaleFloat3().z + outlineScale );
	cube.Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );

	// rescale the cube and draw using the appropriate shaders and textures
	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	cube.SetScale( cube.GetScaleFloat3().x - outlineScale,
		cube.GetScaleFloat3().y - outlineScale, cube.GetScaleFloat3().z - outlineScale );
	stencils["Off"]->Bind( *this );
	cube.Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );
}

void Graphics::DrawWithOutline( RenderableGameObject& object, const XMFLOAT3& color )
{
	// write pixels to the buffer, which will act as the stencil mask
	stencils["Write"]->Bind( *this );
	object.Draw( cameras[cameraToUse] );

	// scale the model and draw the stencil outline, ignoring the pixels previously written to the buffer
	cb_ps_outline.data.outlineColor = color;
    if ( !cb_ps_outline.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_outline.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_outline, pixelShader_outline );
	stencils["Mask"]->Bind( *this );
	object.SetScale( object.GetScaleFloat3().x + outlineScale, 1.0f, object.GetScaleFloat3().z + outlineScale );
	object.Draw( cameras[cameraToUse] );

	// rescale the model and draw using the appropriate shaders and textures
	if ( !cb_ps_point.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_light );
	object.SetScale( object.GetScaleFloat3().x - outlineScale, 1.0f, object.GetScaleFloat3().z - outlineScale );
	stencils["Off"]->Bind( *this );
	object.Draw( cameras[cameraToUse] );
}