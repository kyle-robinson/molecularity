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

// ui
#include <Graphics/UI_Manager.h>
#include <UI/HUD_UI.h>
#include <UI/Pause.h>
#include <UI/Settings_Menu_UI.h>

// "CCTV Camera" (https://skfb.ly/6SD7C) by Smoggybeard is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

bool LevelContainer::Initialize( Graphics* gfx, CameraController* camera, ImGuiManager* imgui, UI_Manager* UI )
{
	graphics = gfx;
	cameras = camera;
	this->imgui = imgui;
	_UiManager = UI;
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
			pointLight.SetInitialPosition( 0.0f, 15.0f, 0.0f );
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
			HRESULT hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\mesh.png", nullptr, boxTextures[BoxType::Mesh].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\wood.png", nullptr, boxTextures[BoxType::Wood].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\iron.jpg", nullptr, boxTextures[BoxType::Iron].GetAddressOf() );
			hr = CreateWICTextureFromFile(graphics->device.Get(), L"Resources\\Textures\\crates\\dCube.png", nullptr, boxTextures[BoxType::DissCube].GetAddressOf());
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
		graphics->context->PSSetShader( graphics->pixelShader_light.GetShader(), NULL, 0 );
	}
}

void LevelContainer::ShowEndLeveLScreen()
{
	if (levelCompleted) {
		//game end
		_UiManager->HideAllUI();
		_UiManager->ShowUi("EndLevel");
		EventSystem::Instance()->AddEvent(EVENTID::GameEndLevelEvent);
		
	}
}

void LevelContainer::RenderFrame()
{
	// CYBERGUN / SPOTLIGHT
	{
		// w/ normals
		GetStencilOutline()->DrawWithOutline( *graphics, spotLight, pointLight.GetConstantBuffer() );
	}

	// DRAWABLES
	{
		// CUBES
		for ( uint32_t i = 0; i < numOfCubes; i++ )
		{
			// render backfaces
			if ( cubes[i]->GetEditableProperties()->GetBoxType() == BoxType::Mesh )
				graphics->GetRasterizer( "Skybox" )->Bind( *graphics );

			if ( cubes[i]->GetIsHovering() )
			{				
				// render with outline
				GetStencilOutline()->DrawWithOutline( *graphics, *cubes[i], cb_vs_matrix,
					pointLight.GetConstantBuffer(), boxTextures[cubes[i]->GetEditableProperties()->GetBoxType()].Get() );
			}
			else
			{
				cubes[i]->Draw( cb_vs_matrix, boxTextures[cubes[i]->GetEditableProperties()->GetBoxType()].Get() );
			}

			// re-enable back-face culling
			graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );
		}
	}
}

void LevelContainer::EndFrame()
{
	_UiManager->Draw(graphics->vertexShader_2D, graphics->pixelShader_2D, &cb_ps_scene);
	
	// setup RTT and update post-processing
	graphics->RenderSceneToTexture();
	postProcessing->Bind( *graphics );

	// spawn imgui windows
	if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
	{
		imgui->BeginRender();
		imgui->SpawnInstructionWindow();
		imgui->SpawnGraphicsWindow( *graphics );
		imgui->SpawnPerformanceWindow();
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

	// update ui components

	_UiManager->Update(dt);

	tool->Update();

	// update camera position for 3D sound
	Sound::Instance()->UpdatePosition( cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionFloat3(), cameras->GetCamera( cameras->GetCurrentCamera() )->GetRotationFloat3().y );
	ShowEndLeveLScreen();
}

void LevelContainer::LateUpdate( const float dt )
{
	// update cubes
	for ( uint32_t i = 0; i < numOfCubes; i++ )
	{
		cubes[i]->SetCamPos(cameras->GetCamera(cameras->GetCurrentCamera())->GetPositionFloat3());
		// update cube scale multiplier
		if ( tool->GetTooltype() == ToolType::Resize )
			cubes[i]->SetScale( static_cast<float>( cubes[i]->GetEditableProperties()->GetSizeMultiplier() ) );

		// cube range collision check
		cubes[i]->SetIsInRange( Collisions::CheckCollisionCircle( cameras->GetCamera( cameras->GetCurrentCamera() ), *cubes[i], 5.0f ) );

		// update objects
		cubes[i]->Update( dt );

		isDissCube = cubes[i]->GetIsDissCube();
		// cube pickup text
		if ( cubes[i]->GetIsInRange() && cubes[i]->GetIsHovering() && !cubes[i]->GetIsHolding() )
		{
			EventSystem::Instance()->AddEvent( EVENTID::CubePickupEvent, ( void* )true );
			EventSystem::Instance()->AddEvent(EVENTID::IsDissCube, &isDissCube);
		}
		else
		{
			EventSystem::Instance()->AddEvent( EVENTID::CubePickupEvent, ( void* )false );
		}
	}

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras->GetCamera( JSON::CameraType::Default ) );
}

void LevelContainer::UpdateCubes( float xPos, float yPos, float zPos, float spacing )
{
	cubes.clear();

	for (uint32_t i = 0; i < numOfCubes; i++)
	{
		std::shared_ptr<Cube> cube = std::make_shared<Cube>();
		if (!cube->Initialize(graphics->context.Get(), graphics->device.Get())) return;

		cube->SetInitialPosition(xPos + ( i * spacing ) , yPos, zPos);

		cubes.push_back(std::move(cube));
	}
}
