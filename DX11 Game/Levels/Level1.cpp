#include "stdafx.h"
#include "Level1.h"
#include "Billboard.h"
#include "Collisions.h"
#include "Rasterizer.h"

Level1::Level1( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) { }

bool Level1::OnCreate()
{
	try
	{
		// DRAWABLES
		{
			// models
			if ( !room.Initialize( "Resources\\Models\\Levels\\Level1-Final.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			room.SetInitialScale( 0.005f, 0.005f, 0.005f );
			room.SetInitialPosition( -2.0f, 0.0f, -20.0f );
			room.SetInitialRotation( 0.0f, XM_PI, 0.0f );

			if ( !pressurePlate.Initialize( "Resources\\Models\\PressurePlate.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			pressurePlate.SetInitialPosition( 0.0f, 0.0f, 24.5f );
			pressurePlate.SetInitialScale( 0.025f, 0.025f, 0.025f );

			// security camera
			if ( !securityCamera.Initialize( "Resources\\Models\\Camera\\scene.gltf", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			securityCamera.SetInitialScale( 2.0f, 2.0f, 2.0f );
			securityCamera.SetInitialPosition( 17.0f, 10.0f, 27.5f );

			//add level UI 
			HUD = make_shared<HUD_UI>();
			PauseUI = make_shared<Pause>();
			TutorialUI= make_shared<Tutorial_UI>();
			EndLevelUI= make_shared<EndLevelScreen_UI>();
		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Level1::OnSwitch()
{	
	levelCompleted = false;
	
	// Update Level System

	CurrentLevel = 1;
	EventSystem::Instance()->AddEvent(EVENTID::SetCurrentLevelEvent, &CurrentLevel);
	
	levelName = "Level1";
	numOfCubes = 1;
	LevelContainer::UpdateCubes( 0.0f, 0.0f, -4.0f );
	NextLevel = 2;
	EventSystem::Instance()->AddEvent(EVENTID::SetNextLevelEvent, &NextLevel);

	// Update HUD with tool data
	EventSystem::Instance()->AddEvent(EVENTID::ToolModeEvent, tool);

	// Initialize UI
	_UiManager->RemoveUI( "MainMenu" );

	_UiManager->AddUi( HUD, "HUD" );
	_UiManager->AddUi(TutorialUI, "Tutorial");
	_UiManager->AddUi( PauseUI, "Pause" );
	_UiManager->AddUi(EndLevelUI, "EndLevel");

	_UiManager->Initialize( graphics->device.Get(), graphics->context.Get(), &cb_vs_matrix_2d );
	_UiManager->HideUi("EndLevel");

	// Initialise Sounds
	Sound::Instance()->InitialiseMusicTrack( "TutorialMusic" );
	Sound::Instance()->InitialiseSoundGroup( "Player" );
	Sound::Instance()->InitialiseSoundGroup( "Cube" );
	Sound::Instance()->InitialiseSoundEffect( "PressurePlateClick" );
	Sound::Instance()->InitialiseSoundEffect( "MenuClick" );
	Sound::Instance()->InitialiseSoundEffect( "Notification" );

	Sound::Instance()->PlayMusic( "TutorialMusic" );
	Sound::Instance()->PlaySoundEffect( "Notification" );

	// Initialize Camera Positions
	cameras->GetCamera( JSON::CameraType::Default )->SetInitialPosition( 0.0f, 7.0f, -20.0f );
	cameras->GetCamera( JSON::CameraType::Static )->SetInitialPosition( 16.0f, 10.0f, 26.0f );
	cameras->GetCamera( JSON::CameraType::Debug )->SetInitialPosition( 0.0f, 7.0f, -15.0f );
}

void Level1::Render()
{
	// Render to sub viewport first using static camera
	GetMultiViewport()->SetUsingSub();
	BeginFrame();
	RenderFrame();

	// Render main scene next with main/debug camera
	GetMultiViewport()->SetUsingMain();
	BeginFrame();
	RenderFrame();

	// Render UI and present the complete frame
	EndFrame();
}

void Level1::RenderFrame()
{
	// render ligths/skysphere
	LevelContainer::RenderFrameEarly();

	// DRAWABLES
	{
		graphics->GetRasterizer( "Skybox" )->Bind( *graphics );
		room.Draw();
		graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );
		pressurePlate.Draw();
		securityCamera.Draw();

		// render the cubes
		LevelContainer::RenderFrame();
	}

	// SPRITES
	{
		if ( cameras->GetCurrentCamera() != JSON::CameraType::Static )
		{
			Shaders::BindShaders( graphics->context.Get(), graphics->vertexShader_2D, graphics->pixelShader_2D );
			cb_ps_scene.data.useTexture = TRUE;
			if ( !cb_ps_scene.ApplyChanges() ) return;
			graphics->context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );

			// render sprites here...
		}
	}
}

void Level1::Update( const float dt )
{
	LevelContainer::Update( dt );

	// adjust pressure plate x-position over time
	static float offset = 0.1f;
	if ( pressurePlate.GetPositionFloat3().x > 13.0f )
		offset = -offset;
	else if ( pressurePlate.GetPositionFloat3().x < -13.0f )
		offset = 0.1f;
	pressurePlate.AdjustPosition( offset, 0.0f, 0.0f );

	// COLLISIONS
	{
		// camera collisions w room
		Collisions::CheckCollisionLevel1( cameras->GetCamera( JSON::CameraType::Default ), room, 19.0f );

		// cube collisions
		for ( uint32_t i = 0; i < numOfCubes; i++ )
		{
			// update collisions w pressure plate
			if ( cubes[i]->CheckCollisionAABB( pressurePlate, dt ) )
			{
				cubes[i]->AdjustPosition( offset, 0.0f, 0.0f );
				if ( cubes[i]->GetPhysicsModel()->GetMass() > 100.0f && !levelCompleted )
				{
					levelCompleted = true;
					Sound::Instance()->PlaySoundEffect( "PressurePlateClick", false, pressurePlate.GetPositionFloat3(), 15.0f );
				}
			}

			// update collisions w other cubes
			for ( uint32_t j = 0; j < numOfCubes; j++ )
				if ( i != j )
					cubes[i]->CheckCollisionAABB( cubes[j], dt );

			// update collisions w room
			Collisions::CheckCollisionLevel1( cubes[i], room, 15.0f );
		}
	}
  
	// set rotation of security camera
	float rotation = Billboard::BillboardModel( cameras->GetCamera( cameras->GetCurrentCamera() ), securityCamera );
	securityCamera.SetRotation( 0.0f, rotation, 0.0f );
  
	LevelContainer::LateUpdate( dt );
}