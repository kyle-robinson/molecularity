#include "Graphics.h"
#include "Rasterizer.h"
#include "Collisions.h"
#include "StencilOutline.h"

bool Graphics::Initialize( HWND hWnd, int width, int height )
{
	if ( !InitializeGraphics( hWnd, width, height ) ) return false;
	if ( !InitializeScene() ) return false;
	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		// models
		if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );

		if ( !skysphere.Initialize( "Resources\\Models\\Sphere\\sphere.obj", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		skysphere.SetInitialScale( 250.0f, 250.0f, 250.0f );

		// lights
		if ( !pointLight.Initialize( "Resources\\Models\\Disco\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		pointLight.SetInitialPosition( -5.0f, 9.0f, -10.0f );
		pointLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

		if ( !directionalLight.Initialize( "Resources\\Models\\Disco\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		directionalLight.SetInitialPosition( 10.0f, 20.0f, 10.0f );
		directionalLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

		if ( !spotLight.Initialize( "Resources\\Models\\Flashlight.fbx", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
		spotLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

		// primitibes
		if ( !cube.Initialize( context.Get(), device.Get() ) ) return false;
		cube.SetInitialPosition( 0.0f, 5.0f, 5.0f );

		if ( !simpleQuad.Initialize( context.Get(), device.Get() ) ) return false;
		simpleQuad.SetInitialPosition( 0.0f, 5.0f, -5.0f );
		simpleQuad.SetInitialRotation( simpleQuad.GetRotationFloat3().x + XM_PI, simpleQuad.GetRotationFloat3().y + XM_PI, simpleQuad.GetRotationFloat3().z );

		// sprites
		if ( !crosshair.Initialize( device.Get(), context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
        crosshair.SetInitialPosition( GetWidth() / 2 - crosshair.GetWidth() / 2, GetHeight() / 2 - crosshair.GetHeight() / 2, 0 );

		// cameras
		XMFLOAT2 aspectRatio = { static_cast<float>( GetWidth() ), static_cast<float>( GetHeight() ) };
		cameras.emplace( "Default", std::make_unique<Camera>( 0.0f, 9.0f, -15.0f ) );
		cameras.emplace( "Static", std::make_unique<Camera>( 0.0f, 9.0f, 0.0f ) );
		cameras.emplace( "Debug", std::make_unique<Camera>( 0.0f, 9.0f, -10.0f ) );
		for ( const auto& cam : cameras )
			cam.second->SetProjectionValues( 70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f );
		camera2D.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );

		// textures
		HRESULT hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\basic_crate.png", nullptr, boxTextures["Basic"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\bounce_crate.png", nullptr, boxTextures["Bounce"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\arrow_crate.png", nullptr, boxTextures["Arrow"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\tnt_crate.png", nullptr, boxTextures["TNT"].GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\brickwall.jpg", nullptr, brickwallTexture.GetAddressOf() );
		hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\brickwall_normal.jpg", nullptr, brickwallNormalTexture.GetAddressOf() );
        COM_ERROR_IF_FAILED( hr, "Failed to create texture from file!" );

		// constant buffers
		hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
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
	ClearScene();
	UpdateRenderState();
	UpdateConstantBuffers();
}

void Graphics::RenderFrame()
{
	RenderSkySphere();
	RenderLights();
	RenderModels();
	RenderPrimitives();
	RenderSprites();
}

void Graphics::EndFrame()
{
	RenderSceneText();
	RenderImGuiWindows();
	PresentScene();
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

void Graphics::UpdateConstantBuffers()
{
	stencilOutline->SetOutlineColor( outlineColor );
	stencilOutline->SetOutlineScale( outlineScale );
	
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

	Model::BindMatrices( context.Get(), cb_vs_matrix, cameras[cameraToUse] );
}

void Graphics::RenderSkySphere()
{
	// render skysphere first
	Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_noLight );
	rasterizers["Skybox"]->Bind( *this );
	skysphere.Draw();
	rasterizers[rasterizerSolid ? "Solid" : "Wireframe"]->Bind( *this );
}

void Graphics::RenderLights()
{
	// render models w/out normals
	pointLight.Draw();
	directionalLight.Draw();
	
	// render models w/ normals
	context->PSSetShader( pixelShader_light.GetShader(), NULL, 0 );
	spotLight.Draw();
}

void Graphics::RenderModels()
{
	hubRoom.Draw();
}

void Graphics::RenderPrimitives()
{
	// render models w/ normal maps
	simpleQuad.Draw( cb_vs_matrix, cb_ps_scene, brickwallTexture.Get(), brickwallNormalTexture.Get() );

	// render objects w/ stencils
	if ( cubeHover )
		stencilOutline->DrawWithOutline( *this, cube, cb_vs_matrix, boxTextures[selectedBox].Get() );
	else
		cube.Draw( cb_vs_matrix, boxTextures[selectedBox].Get() );
}

void Graphics::RenderSprites()
{
	Shaders::BindShaders( context.Get(), vertexShader_2D, pixelShader_2D );
	cb_ps_scene.data.useTexture = true;
    if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
    crosshair.Draw( camera2D.GetWorldOrthoMatrix() );
}