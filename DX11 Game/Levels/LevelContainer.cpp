#include "stdafx.h"
#include "LevelContainer.h"

// bindables
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

// systems
#include "Fog.h"
#include "Collisions.h"
#include "ImGuiManager.h"
#include "TextRenderer.h"
#include "PostProcessing.h"
#include "StencilOutline.h"
#include <dxtk/WICTextureLoader.h>

bool LevelContainer::Initialize( Graphics* gfx, CameraController* camera, ImGuiManager* imgui )
{
	graphics = gfx;
	cameras = camera;
	this->imgui = imgui;
	if ( !InitializeScene() )
		return false;
	return true;
}

bool LevelContainer::InitializeScene()
{
	try
	{
		// DRAWABLES
		{
			// cubes
			for ( uint32_t i = 0; i < NUM_CUBES; i++ )
			{
				std::shared_ptr<Cube> cube = std::make_shared<Cube>();
				if ( !cube->Initialize( graphics->context.Get(), graphics->device.Get() ) ) return false;

				float xPos = 2.5f;
				if ( i % 2 == 0 ) xPos = -xPos;
				cube->SetInitialPosition( xPos, 0.0f, -6.0f );

				cubes.push_back( std::move( cube ) );
			}

			// skysphere
			if ( !skysphere.Initialize( "Resources\\Models\\Sphere\\sphere.obj", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			skysphere.SetInitialScale( 250.0f, 250.0f, 250.0f );
		}

		// LIGHTS
		{
			if ( !directionalLight.Initialize( *graphics, cb_vs_matrix ) ) return false;
			directionalLight.SetInitialPosition( 10.0f, 20.0f, 10.0f );
			directionalLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

			if ( !pointLight.Initialize( *graphics, cb_vs_matrix ) ) return false;
			pointLight.SetInitialPosition( -5.0f, 9.0f, -10.0f );
			pointLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

			if ( !spotLight.Initialize( *graphics, cb_vs_matrix ) ) return false;
			spotLight.SetInitialScale( 0.01f, 0.01f, 0.01f );			
		}

		// SYSTEMS
		{
			postProcessing = std::make_shared<PostProcessing>( *graphics );
			stencilOutline = std::make_shared<StencilOutline>( *graphics );
			textRenderer = std::make_shared<TextRenderer>( *graphics );
			multiViewport = std::make_shared<MultiViewport>();
			fogSystem = std::make_shared<Fog>( *graphics );
		}

		// CONSTANT BUFFERS
		{
			HRESULT hr = cb_vs_matrix_2d.Initialize( graphics->device.Get(), graphics->context.Get() );
			hr = cb_vs_matrix.Initialize( graphics->device.Get(), graphics->context.Get() );
			hr = cb_ps_scene.Initialize( graphics->device.Get(), graphics->context.Get() );
			COM_ERROR_IF_FAILED( hr, "Failed to initialize constant buffer!" );
		}

		// TEXTURES
		{
			HRESULT hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\basic_crate.png", nullptr, boxTextures[BoxType::Default].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\bounce_crate.png", nullptr, boxTextures[BoxType::Bounce].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\arrow_crate.png", nullptr, boxTextures[BoxType::Arrow].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\tnt_crate.png", nullptr, boxTextures[BoxType::TNT].GetAddressOf() );
			COM_ERROR_IF_FAILED( hr, "Failed to create texture from file!" );
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
void LevelContainer::BeginFrame()
{
	// setup viewports and pipeline state
	if ( multiViewport->IsUsingSub() )
		graphics->ClearScene();
	graphics->UpdateRenderState();
	multiViewport->Update( *this );

	// update constant buffers
	fogSystem->UpdateConstantBuffer( *graphics );
	
	cb_ps_scene.data.useNormalMap = FALSE;
	cb_ps_scene.data.useTexture = graphics->useTexture;
	cb_ps_scene.data.alphaFactor = graphics->alphaFactor;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	graphics->context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );

	pointLight.UpdateConstantBuffer( *graphics );
	directionalLight.UpdateConstantBuffer( *graphics );
	spotLight.UpdateConstantBuffer( *graphics, cameras->GetCamera( JSON::CameraType::Default ) );

	// bind camera matrices
	Model::BindMatrices( graphics->context.Get(), cb_vs_matrix, cameras->GetCamera( cameras->GetCurrentCamera() ) );
}

void LevelContainer::RenderFrameEarly()
{
	// SKYSPHERE
	{
		Shaders::BindShaders( graphics->context.Get(), graphics->vertexShader_light, graphics->pixelShader_noLight );
		graphics->GetRasterizer( "Skybox" )->Bind( *graphics );
		skysphere.Draw();
		graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );
	}

	// LIGHTS
	{
		// w/out normals
		pointLight.Draw();
		directionalLight.Draw();

		// w/ normals
		graphics->context->PSSetShader( graphics->pixelShader_light.GetShader(), NULL, 0 );
		spotLight.Draw();
	}
}

void LevelContainer::RenderFrame()
{
	// CUBES
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		if ( cubes[i]->GetIsHovering() )
		{
			GetStencilOutline()->DrawWithOutline( *graphics, *cubes[i], cb_vs_matrix,
				pointLight.GetConstantBuffer(), boxTextures[cubes[i]->GetEditableProperties()->GetBoxType()].Get() );
		}
		else
		{
			cubes[i]->Draw( cb_vs_matrix, boxTextures[cubes[i]->GetEditableProperties()->GetBoxType()].Get() );
		}
	}
}

void LevelContainer::EndFrame()
{
	// setup RTT and update post-processing
	graphics->RenderSceneToTexture();
	postProcessing->Bind( *graphics );

	// update text rendering
	textRenderer->RenderCubeMoveText( *this );
	textRenderer->RenderMultiToolText( *this );
	textRenderer->RenderCameraText( *this );

	// spawn imgui windows
	if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
	{
		imgui->BeginRender();
		imgui->SpawnInstructionWindow();
		imgui->SpawnGraphicsWindow( *graphics );
		directionalLight.SpawnControlWindow();
		pointLight.SpawnControlWindow();
		spotLight.SpawnControlWindow();
		fogSystem->SpawnControlWindow();
		stencilOutline->SpawnControlWindow();
		postProcessing->SpawnControlWindow();
		imgui->EndRender();
	}

	graphics->PresentScene();
}

void LevelContainer::Update( const float dt )
{
	// update lights
	pointLight.SetPosition( pointLight.GetLightPosition() );
	directionalLight.SetPosition( directionalLight.GetLightPosition() );

	// update skysphere
	skysphere.SetPosition( cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionFloat3() );	
}

void LevelContainer::LateUpdate( const float dt )
{
	// update cubes
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		// update cube scale multiplier
		if ( cubes[i]->GetEditableProperties()->GetToolType() == ToolType::Resize )
			cubes[i]->SetScale( static_cast< float >( cubes[i]->GetEditableProperties()->GetSizeMultiplier() ) );

		// cube range collision check
		cubes[i]->SetIsInRange( Collisions::CheckCollisionSphere( cameras->GetCamera( cameras->GetCurrentCamera() ), *cubes[i], 5.0f ) );

		// update objects
		cubes[i]->Update( dt );
	}

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras->GetCamera( JSON::CameraType::Default ) );
}

void LevelContainer::ProcessInput()
{
	// update main input here...
}