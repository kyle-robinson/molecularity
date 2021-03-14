#include "Graphics.h"
#include "Collisions.h"

// bindables
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

// systems
#include "Fog.h"
#include "TextRenderer.h"
#include "PostProcessing.h"
#include "StencilOutline.h"

bool Graphics::Initialize( HWND hWnd, CameraController* camera, int width, int height )
{
	if ( !InitializeGraphics( hWnd, width, height ) ) return false;
	imgui.Initialize( hWnd, device.Get(), context.Get() );
	if ( !InitializeScene() ) return false;
	this->cameras = camera;

	UI_Controllor.Initialize(device.Get(), context.Get());
	UI_Controllor.getCustomUi()->setcb_ps_scene(cb_ps_scene);
	UI_Controllor.getCustomUi()->setcb_vs_matrix_2d(cb_vs_matrix_2d);
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

			if ( !pressurePlate.Initialize( "Resources\\Models\\PressurePlate.fbx", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
			pressurePlate.SetInitialPosition( 0.0f, 0.0f, 15.0f );
			pressurePlate.SetInitialScale( 0.025f, 0.025f, 0.025f );

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
			//Moved To ui
			/*if ( !crosshair.Initialize( device.Get(), context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
			crosshair.SetInitialPosition( GetWidth() / 2 - crosshair.GetWidth() / 2, GetHeight() / 2 - crosshair.GetHeight() / 2, 0 );*/
		}

		// SYSTEMS
		{
			postProcessing = std::make_shared<PostProcessing>( *this );
			stencilOutline = std::make_shared<StencilOutline>( *this );
			textRenderer = std::make_shared<TextRenderer>( *this );
			multiViewport = std::make_shared<MultiViewport>();
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
	// setup viewports and pipeline state
	if ( multiViewport->IsUsingSub() )
		ClearScene();
	UpdateRenderState();
	multiViewport->Update( *this );

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
		GetRasterizer( "Skybox" )->Bind( *this );
		skysphere.Draw();
		GetRasterizer( rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *this );
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
		pressurePlate.Draw();

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
		if ( cameras->GetCurrentCamera() != JSON::CameraType::Static )
		{
			
		}
	}
}

void Graphics::EndFrame()
{
	
	UI_Controllor.Draw(this, vertexShader_2D, pixelShader_2D);
	// setup RTT and update post-processing
	RenderSceneToTexture();
	postProcessing->Bind( *this );

	

	// spawn imgui windows
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
		postProcessing->SpawnControlWindow();
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
	if ( !Collisions::CheckCollisionCircle( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, 25.0f ) )
		cameras->CollisionResolution( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, dt );

	// update cube scale multiplier
	if ( cube.GetEditableProperties()->GetToolType() == ToolType::Resize )
		cube.SetScale( static_cast<float>( cube.GetEditableProperties()->GetSizeMultiplier() ) );

	// cube range collision check
	cube.SetIsInRange( Collisions::CheckCollisionSphere( cameras->GetCamera( cameras->GetCurrentCamera() ), cube, 5.0f ) );

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras->GetCamera( JSON::CameraType::Default ) );

	// update objects
	cube.Update( dt );
	cube.CheckCollisionAABB( pressurePlate, dt );
}