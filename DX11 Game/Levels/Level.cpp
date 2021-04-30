#include "stdafx.h"
#include "Level.h"

// bindables
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

// systems
#include "Fog.h"
#include "ImGuiManager.h"
#include "TextRenderer.h"
#include "PostProcessing.h"
#include "StencilOutline.h"

bool Level::Initialize( Graphics* gfx, CameraController* camera, ImGuiManager* imgui )
{
	graphics = gfx;
	cameras = camera;
	this->imgui = imgui;
	if ( !InitializeScene() )
		return false;
	return true;
}

bool Level::InitializeScene()
{
	try
	{
		// CUBES
		{
			// primitives
			for ( uint32_t i = 0; i < NUM_CUBES; i++ )
			{
				std::shared_ptr<Cube> cube = std::make_shared<Cube>();
				if ( !cube->Initialize( graphics->context.Get(), graphics->device.Get() ) ) return false;

				float xPos = 2.5f;
				if ( i % 2 == 0 ) xPos = -xPos;
				cube->SetInitialPosition( xPos, 0.0f, -6.0f );

				cubes.push_back( std::move( cube ) );
			}

			// lights
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
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

// RENDER PIPELINE
void Level::BeginFrame()
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

void Level::EndFrame()
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