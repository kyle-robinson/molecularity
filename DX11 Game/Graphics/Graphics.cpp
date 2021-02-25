#include "Graphics.h"
#include "Collisions.h"
#include "Rasterizer.h"
#include "TextRenderer.h"
#include "StencilOutline.h"

bool Graphics::Initialize( HWND hWnd, int width, int height )
{
	if ( !InitializeGraphics( hWnd, width, height ) ) return false;
	imgui.Initialize( hWnd, device.Get(), context.Get() );
	if ( !InitializeScene() ) return false;
	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		// DRAWABLES
		{
			// models
			if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
			hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );

			if ( !skysphere.Initialize( "Resources\\Models\\Sphere\\sphere.obj", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
			skysphere.SetInitialScale( 250.0f, 250.0f, 250.0f );

			// lights
			if ( !directionalLight.Initialize( *this, cb_vs_matrix ) ) return false;
			directionalLight.SetInitialPosition( 10.0f, 20.0f, 10.0f );
			directionalLight.SetInitialScale( 0.01f, 0.01f, 0.01f );
			
			if ( !pointLight.Initialize( *this, cb_vs_matrix ) ) return false;
			pointLight.SetInitialPosition( -5.0f, 9.0f, -10.0f );
			pointLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

			if ( !spotLight.Initialize( *this, cb_vs_matrix ) ) return false;
			spotLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

			// primitives
			if ( !cube.Initialize( context.Get(), device.Get() ) ) return false;
			cube.SetInitialPosition( 0.0f, 10.0f, -6.0f );

			if ( !simpleQuad.Initialize( context.Get(), device.Get() ) ) return false;
			simpleQuad.SetInitialPosition( 0.0f, 5.0f, 5.0f );
			simpleQuad.SetInitialRotation( simpleQuad.GetRotationFloat3().x + XM_PI, simpleQuad.GetRotationFloat3().y + XM_PI, simpleQuad.GetRotationFloat3().z );

			// sprites
			if ( !crosshair.Initialize( device.Get(), context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
			crosshair.SetInitialPosition( GetWidth() / 2 - crosshair.GetWidth() / 2, GetHeight() / 2 - crosshair.GetHeight() / 2, 0 );
		}

		// CAMERAS
		{
			XMFLOAT2 aspectRatio = { static_cast<float>( GetWidth() ), static_cast<float>( GetHeight() ) };
			cameras.emplace( JSON::CameraType::Default, std::make_unique<Camera>( 0.0f, 9.0f, -15.0f ) );
			cameras.emplace( JSON::CameraType::Static, std::make_unique<Camera>( 0.0f, 9.0f, 0.0f ) );
			cameras.emplace( JSON::CameraType::Debug, std::make_unique<Camera>( 0.0f, 9.0f, -10.0f ) );
			for ( const auto& cam : cameras )
				cam.second->SetProjectionValues( 70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f );
			camera2D.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );
		}

		// SYSTEMS
		{
			stencilOutline = std::make_shared<StencilOutline>( *this );
			textRenderer = std::make_shared<TextRenderer>( *this );
			fogSystem = std::make_shared<Fog>( *this );
		}

		// TEXTURES
		{
			HRESULT hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\basic_crate.png", nullptr, boxTextures[BoxType::Default].GetAddressOf() );
			hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\bounce_crate.png", nullptr, boxTextures[BoxType::Bounce].GetAddressOf() );
			hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\arrow_crate.png", nullptr, boxTextures[BoxType::Arrow].GetAddressOf() );
			hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\crates\\tnt_crate.png", nullptr, boxTextures[BoxType::TNT].GetAddressOf() );
			hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\brickwall.jpg", nullptr, brickwallTexture.GetAddressOf() );
			hr = CreateWICTextureFromFile( device.Get(), L"Resources\\Textures\\brickwall_normal.jpg", nullptr, brickwallNormalTexture.GetAddressOf() );
			COM_ERROR_IF_FAILED( hr, "Failed to create texture from file!" );
		}

		// CONSTANT BUFFERS
		{
			HRESULT hr = cb_vs_matrix_2d.Initialize( device.Get(), context.Get() );
			hr = cb_vs_matrix.Initialize( device.Get(), context.Get() );
			hr = cb_ps_scene.Initialize( device.Get(), context.Get() );
			COM_ERROR_IF_FAILED( hr, "Failed to initialize constant buffer!" );
		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

// RENDER PIPELINE
void Graphics::BeginFrame()
{
	ClearScene();
	UpdateRenderState();

	// update constant buffers
	fogSystem->UpdateConstantBuffer( *this );
	
	cb_ps_scene.data.useNormalMap = FALSE;
	cb_ps_scene.data.useTexture = useTexture;
	cb_ps_scene.data.alphaFactor = alphaFactor;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );

	pointLight.UpdateConstantBuffer( *this );
	directionalLight.UpdateConstantBuffer( *this );
	spotLight.UpdateConstantBuffer( *this, cameras[JSON::CameraType::Default] );

	// bind camera matrices
	Model::BindMatrices( context.Get(), cb_vs_matrix, cameras[cameraToUse] );
}

void Graphics::RenderFrame()
{
	// SKYSPHERE
	{
		Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_noLight );
		rasterizers["Skybox"]->Bind( *this );
		skysphere.Draw();
		rasterizers[rasterizerSolid ? "Solid" : "Wireframe"]->Bind( *this );
	}

	// LIGHTS
	{
		// w/out normals
		pointLight.Draw();
		directionalLight.Draw();
	
		// w/ normals
		context->PSSetShader( pixelShader_light.GetShader(), NULL, 0 );
		spotLight.Draw();
	}

	// DRAWABLES
	{
		hubRoom.Draw();

		// w/ normal maps
		simpleQuad.Draw( cb_vs_matrix, cb_ps_scene, brickwallTexture.Get(), brickwallNormalTexture.Get() );

		// w/ stencils
		if ( cube.GetIsHovering() )
		{
			stencilOutline->DrawWithOutline( *this, cube, cb_vs_matrix,
				pointLight.GetConstantBuffer(), boxTextures[cube.GetBoxType()].Get() );
		}
		else
		{
			cube.Draw( cb_vs_matrix, boxTextures[cube.GetBoxType()].Get() );
		}
	}

	// SPRITES
	{
		Shaders::BindShaders( context.Get(), vertexShader_2D, pixelShader_2D );
		cb_ps_scene.data.useTexture = TRUE;
		if ( !cb_ps_scene.ApplyChanges() ) return;
		context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
		crosshair.Draw( camera2D.GetWorldOrthoMatrix() );
	}
}

void Graphics::EndFrame()
{
	textRenderer->RenderCubeMoveText( *this );
	textRenderer->RenderMultiToolText( *this );
	textRenderer->RenderCameraText( *this );

	if ( cameraToUse == JSON::CameraType::Debug )
	{
		imgui.BeginRender();
		imgui.SpawnInstructionWindow();
		imgui.SpawnGraphicsWindow( *this );
		directionalLight.SpawnControlWindow();
		pointLight.SpawnControlWindow();
		spotLight.SpawnControlWindow();
		fogSystem->SpawnControlWindow();
		stencilOutline->SpawnControlWindow();
		imgui.EndRender();
	}

	PresentScene();
}

void Graphics::Update( const float dt )
{
	// update lights/skysphere
	pointLight.SetPosition( pointLight.GetLightPosition() );
	directionalLight.SetPosition( directionalLight.GetLightPosition() );
	skysphere.SetPosition( cameras[cameraToUse]->GetPositionFloat3() );

	// camera world collisions
	bool wallCollision = Collisions::CheckCollisionCircle( cameras[JSON::CameraType::Default], hubRoom, 25.0f );
	if ( !wallCollision )
	{
		float dx = hubRoom.GetPositionFloat3().x - cameras[JSON::CameraType::Default]->GetPositionFloat3().x;
		float dz = hubRoom.GetPositionFloat3().z - cameras[JSON::CameraType::Default]->GetPositionFloat3().z;
		float length = std::sqrtf( dx * dx + dz * dz );
		dx /= length;
		dz /= length;
		dx *= cameras[JSON::CameraType::Default]->GetCameraSpeed() * 10.0f;
		dz *= cameras[JSON::CameraType::Default]->GetCameraSpeed() * 10.0f;
		cameras[JSON::CameraType::Default]->AdjustPosition( dx, 0.0f, dz );
	}

	// cube range collision check
	if ( cube.GetEditableProperties()->GetType() == ToolType::RESIZE )
		cube.SetScale( cube.GetSizeToUse(), cube.GetSizeToUse(), cube.GetSizeToUse() );
	cube.SetIsInRange( Collisions::CheckCollisionSphere( cameras[cameraToUse], cube, 5.0f ) );

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras[JSON::CameraType::Default] );

	//update object physics
	cube.UpdatePhysics( dt );
}