#include "stdafx.h"
#include "Level.h"

// bindables
#include "Rasterizer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

// systems
#include "Fog.h"
#include "TextRenderer.h"
#include "PostProcessing.h"
#include "StencilOutline.h"

bool Level::Initialize( HWND hWnd, CameraController* camera, int width, int height )
{
	if ( !InitializeGraphics( hWnd, width, height ) ) return false;
	//imgui.Initialize( hWnd, device.Get(), context.Get() );
	if ( !InitializeScene() ) return false;
	this->cameras = camera;

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
				if ( !cube->Initialize( context.Get(), device.Get() ) ) return false;

				float xPos = 2.5f;
				if ( i % 2 == 0 ) xPos = -xPos;
				cube->SetInitialPosition( xPos, 0.0f, -6.0f );

				cubes.push_back( std::move( cube ) );
			}

			// lights
			if ( !directionalLight.Initialize( *this, cb_vs_matrix ) ) return false;
			directionalLight.SetInitialPosition( 10.0f, 20.0f, 10.0f );
			directionalLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

			if ( !pointLight.Initialize( *this, cb_vs_matrix ) ) return false;
			pointLight.SetInitialPosition( -5.0f, 9.0f, -10.0f );
			pointLight.SetInitialScale( 0.01f, 0.01f, 0.01f );

			if ( !spotLight.Initialize( *this, cb_vs_matrix ) ) return false;
			spotLight.SetInitialScale( 0.01f, 0.01f, 0.01f );
		}

		// SYSTEMS
		{
			postProcessing = std::make_shared<PostProcessing>( *this );
			stencilOutline = std::make_shared<StencilOutline>( *this );
			textRenderer = std::make_shared<TextRenderer>( *this );
			multiViewport = std::make_shared<MultiViewport>();
			fogSystem = std::make_shared<Fog>( *this );
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
void Level::BeginFrame()
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

void Level::EndFrame()
{
	// setup RTT and update post-processing
	RenderSceneToTexture();
	postProcessing->Bind( *this );

	// update text rendering
	textRenderer->RenderCubeMoveText( *this );
	textRenderer->RenderMultiToolText( *this );
	textRenderer->RenderCameraText( *this );

	// spawn imgui windows
	/*if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
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
	}*/

	PresentScene();
}