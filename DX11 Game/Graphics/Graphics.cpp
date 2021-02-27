#include "Graphics.h"
#include "Collisions.h"
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "TextRenderer.h"
#include "StencilOutline.h"

bool Graphics::Initialize( HWND hWnd, CameraController* camera, int width, int height )
{
	if ( !InitializeGraphics( hWnd, width, height ) ) return false;
	imgui.Initialize( hWnd, device.Get(), context.Get() );
	if ( !InitializeScene() ) return false;
	this->cameras = camera;

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

			if ( !fullscreen.Initialize( device.Get() ) ) return false;

			// sprites
			if ( !crosshair.Initialize( device.Get(), context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
			crosshair.SetInitialPosition( GetWidth() / 2 - crosshair.GetWidth() / 2, GetHeight() / 2 - crosshair.GetHeight() / 2, 0 );
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
			hr = cb_vs_fullscreen.Initialize( device.Get(), context.Get() );
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
	spotLight.UpdateConstantBuffer( *this, cameras->GetCamera( JSON::CameraType::Default ) );

	// bind camera matrices
	Model::BindMatrices( context.Get(), cb_vs_matrix, cameras->GetCamera( cameras->GetCurrentCamera() ) );
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
				pointLight.GetConstantBuffer(), boxTextures[cube.GetEditableProperties()->GetBoxType()].Get() );
		}
		else
		{
			cube.Draw( cb_vs_matrix, boxTextures[cube.GetEditableProperties()->GetBoxType()].Get() );
		}
	}

	// SPRITES
	{
		Shaders::BindShaders( context.Get(), vertexShader_2D, pixelShader_2D );
		cb_ps_scene.data.useTexture = TRUE;
		if ( !cb_ps_scene.ApplyChanges() ) return;
		context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
		crosshair.Draw( cameras->GetUICamera().GetWorldOrthoMatrix() );
	}
}

void Graphics::EndFrame()
{
	// set and clear back buffer
	static float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    backBuffer->BindAsBuffer( *this, clearColor );

    // render to fullscreen texture
	Shaders::BindShaders( context.Get(), vertexShader_full, pixelShader_full );
    fullscreen.SetupBuffers( context.Get(), cb_vs_fullscreen, TRUE );
    context->PSSetShaderResources( 0u, 1u, renderTarget->GetShaderResourceViewPtr() );
    Bind::Rasterizer::DrawSolid( *this, fullscreen.ib_full.IndexCount() ); // always draw as solid

	textRenderer->RenderCubeMoveText( *this );
	textRenderer->RenderMultiToolText( *this );
	textRenderer->RenderCameraText( *this );

	if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
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
	skysphere.SetPosition( cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionFloat3() );

	// camera world collisions. Will be player object collisions in the future and ideally not here
	bool wallCollision = Collisions::CheckCollisionCircle( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, 25.0f );
	if ( !wallCollision )
	{
		float dx = hubRoom.GetPositionFloat3().x - cameras->GetCamera( JSON::CameraType::Default )->GetPositionFloat3().x;
		float dz = hubRoom.GetPositionFloat3().z - cameras->GetCamera( JSON::CameraType::Default )->GetPositionFloat3().z;
		float length = std::sqrtf( dx * dx + dz * dz );
		dx /= length;
		dz /= length;
		dx *= cameras->GetCamera( JSON::CameraType::Default)->GetCameraSpeed() * 10.0f;
		dz *= cameras->GetCamera( JSON::CameraType::Default)->GetCameraSpeed() * 10.0f;
		cameras->GetCamera( JSON::CameraType::Default)->AdjustPosition( dx, 0.0f, dz );
	}

	// update cube scale multiplier
	if ( cube.GetEditableProperties()->GetToolType() == ToolType::Resize )
		cube.SetScale( static_cast<float>( cube.GetEditableProperties()->GetSizeMultiplier() ) );

	// cube range collision check
	cube.SetIsInRange( Collisions::CheckCollisionSphere( cameras->GetCamera( cameras->GetCurrentCamera() ), cube, 5.0f ) );

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras->GetCamera( JSON::CameraType::Default ) );

	//update object physics
	cube.UpdatePhysics( dt );
}