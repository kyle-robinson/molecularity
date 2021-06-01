#include "stdafx.h"
#include "Level3.h"
#include "Billboard.h"
#include "Collisions.h"
#include "Rasterizer.h"

Level3::Level3( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool Level3::OnCreate()
{
	try
	{
		// DRAWABLES
		{
			// models
			if ( !room.Initialize( "Resources\\Models\\Levels\\Level3-V1.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			room.SetInitialScale( 0.005f, 0.005f, 0.005f );
			room.SetInitialPosition( 0.0f, 0.0f, 0.0f );
			//room.SetInitialPosition( -32.0f, 0.0f, -40.0f );
			//room.SetInitialRotation( 0.0f, XM_PI, 0.0f );

			if ( !door.Initialize( "Resources\\Models\\Levels\\Door.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			door.SetInitialScale( 0.005f, 0.005f, 0.005f );
			door.SetInitialPosition( 0.0f, 0.0f, 0.0f );

			if ( !pressurePlate.Initialize( "Resources\\Models\\PressurePlate.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			pressurePlate.SetInitialPosition( 0.0f, 0.0f, 31.5f );
			pressurePlate.SetInitialScale( 0.025f, 0.025f, 0.025f );

			// security camera
			if ( !securityCamera.Initialize( "Resources\\Models\\Camera\\scene.gltf", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			securityCamera.SetInitialScale( 2.0f, 2.0f, 2.0f );
			securityCamera.SetInitialPosition( 17.5f, 10.0f, 35.0f );
		}

		// UI
		{
			HUD = make_shared<HUD_UI>();
			PauseUI = make_shared<Pause>();
			EndLevelUI = make_shared<EndLevelScreen_UI>();
		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Level3::OnSwitch()
{
	// Update Level System
	levelCompleted = false;
	CurrentLevel = 3;
	EventSystem::Instance()->AddEvent( EVENTID::SetCurrentLevelEvent, &CurrentLevel );

	levelName = "Level3";
	numOfCubes = 2;
	LevelContainer::UpdateCubes();
	NextLevel = 4;

	//UI
	_UiManager->RemoveUI( "MainMenu" );
	_UiManager->RemoveUI( "Tutorial" );
	_UiManager->ShowAllUi();
	_UiManager->HideUi( "EndLevel" );

	EventSystem::Instance()->AddEvent( EVENTID::SetNextLevelEvent, &NextLevel );

	Sound::Instance()->InitialiseMusicTrack( "LevelMusic" );
	Sound::Instance()->InitialiseSoundGroup( "Player" );
	Sound::Instance()->InitialiseSoundGroup( "Cube" );
	Sound::Instance()->PlaySoundEffect( "LevelComplete" );
	Sound::Instance()->InitialiseSoundEffect( "MenuClick" );

	Sound::Instance()->PlayMusic( "LevelMusic" );

	// Initialize Camera Positions
	cameras->GetCamera( JSON::CameraType::Default )->SetInitialPosition( 0.0f, 7.0f, -20.0f );
	cameras->GetCamera( JSON::CameraType::Static )->SetInitialPosition( 16.5f, 10.0f, 33.5f );
	cameras->GetCamera( JSON::CameraType::Debug )->SetInitialPosition( 0.0f, 7.0f, -15.0f );
}

void Level3::Render()
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

void Level3::RenderFrame()
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
}

void Level3::Update( const float dt )
{
	// update lights/skysphere
	LevelContainer::Update( dt );

	// COLLISIONS
	{
		// camera collisions w room
		//Collisions::CheckCollisionLevel3( cameras->GetCamera( JSON::CameraType::Default ), 17.0f );

		// cube collisions
		for ( uint32_t i = 0; i < NUM_CUBES; i++ )
		{
			// update collisions w pressure plate
			if ( cubes[i]->CheckCollisionAABB( pressurePlate, dt ) )
			{
				if ( cubes[i]->GetPhysicsModel()->GetMass() > 100.0f && !levelCompleted )
				{
					levelCompleted = true;
					Sound::Instance()->PlaySoundEffect( "LevelComplete" );
				}
			}

			// update collisions w other cubes
			for ( uint32_t j = 0; j < NUM_CUBES; j++ ) if ( i != j )
				cubes[i]->CheckCollisionAABB( cubes[j], dt );

			// update collisions w room
			//Collisions::CheckCollisionLevel3( cubes[i], 17.0f );
		}
	}

	// set rotation of security camera
	float rotation = Billboard::BillboardModel( cameras->GetCamera( cameras->GetCurrentCamera() ), securityCamera );
	securityCamera.SetRotation( 0.0f, rotation, 0.0f );

	// update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}