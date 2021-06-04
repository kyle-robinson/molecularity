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
		// Renderables
		if ( !ModelData::LoadModelData( "Level1_Objects.json" ) ) return false;
		if ( !ModelData::InitializeModelData( *&graphics->context, *&graphics->device, cb_vs_matrix, renderables ) ) return false;

		// User Interface
		HUD = std::make_shared<HUD_UI>();
		PauseUI = std::make_shared<Pause>();
		TutorialUI = std::make_shared<Tutorial_UI>();
		EndLevelUI = std::make_shared<EndLevelScreen_UI>();
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
	// Update Level System
	levelCompleted = false;
	CurrentLevel = 1;
	EventSystem::Instance()->AddEvent( EVENTID::SetCurrentLevelEvent, &CurrentLevel );

	levelName = "Level1";
	numOfCubes = 1;
	LevelContainer::UpdateCubes( 0.0f, 0.0f, -4.0f );
	NextLevel = 2;
	EventSystem::Instance()->AddEvent( EVENTID::SetNextLevelEvent, &NextLevel );

	// Update Tool
	EventSystem::Instance()->AddEvent( EVENTID::ToolModeEvent, tool );

	// Update UI System
	_UiManager->RemoveUI( "MainMenu" );
	_UiManager->AddUi( HUD, "HUD" );
	_UiManager->AddUi( PauseUI, "Pause" );
	_UiManager->AddUi( TutorialUI, "Tutorial" );
	_UiManager->AddUi( EndLevelUI, "EndLevel" );
	_UiManager->Initialize( graphics->device.Get(), graphics->context.Get(), &cb_vs_matrix_2d );
	_UiManager->HideUi( "EndLevel" );

	// Update Sound System
	Sound::Instance()->InitialiseMusicTrack( "TutorialMusic" );
	Sound::Instance()->InitialiseSoundGroup( "Player" );
	Sound::Instance()->InitialiseSoundGroup( "Cube" );

	Sound::Instance()->InitialiseSoundEffect( "LevelComplete" );
	Sound::Instance()->InitialiseSoundEffect( "MenuClick" );
	Sound::Instance()->InitialiseSoundEffect( "Notification" );

	Sound::Instance()->PlayMusic( "TutorialMusic" );
	Sound::Instance()->PlaySoundEffect( "Notification" );

	// Update Cameras
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
	// Render lights/skysphere
	LevelContainer::RenderFrameEarly();

	// Draw w/ back-face culling
	graphics->GetRasterizer( "Skybox" )->Bind( *graphics );
	renderables["Room"].Draw();
	graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );

	// Draw with back-face culling
	renderables["PressurePlate"].Draw();
	renderables["SecurityCamera"].Draw();

	// Render the cubes
	LevelContainer::RenderFrame();
}

void Level1::Update( const float dt )
{
	LevelContainer::Update( dt );

	// Adjust pressure plate x-position over time
	static float offset = 0.1f;
	if ( renderables["PressurePlate"].GetPositionFloat3().x > 13.0f )
		offset = -offset;
	else if ( renderables["PressurePlate"].GetPositionFloat3().x < -13.0f )
		offset = 0.1f;
	renderables["PressurePlate"].AdjustPosition( offset, 0.0f, 0.0f );

	// Camera collisions w room
	Collisions::CheckCollisionLevel1( cameras->GetCamera( JSON::CameraType::Default ), 18.5f );

	// Cube collisions
	for ( uint32_t i = 0; i < numOfCubes; i++ )
	{
		// Update collisions w pressure plate
		if ( cubes[i]->CheckCollisionAABB( renderables["PressurePlate"], dt ) )
		{
			cubes[i]->AdjustPosition( offset, 0.0f, 0.0f );
			if ( cubes[i]->GetPhysicsModel()->GetMass() >= 240.0f && !levelCompleted )
			{
				levelCompleted = true;
				Sound::Instance()->PlaySoundEffect( "LevelComplete" );
			}
		}

		// Update collisions w other cubes
		for ( uint32_t j = 0; j < numOfCubes; j++ ) if ( i != j )
			cubes[i]->CheckCollisionAABB( cubes[j], dt );

		// Update collisions w room
		Collisions::CheckCollisionLevel1( cubes[i], 15.0f );
	}

	// Set rotation of security camera
	float rotation = Billboard::BillboardModel( cameras->GetCamera( cameras->GetCurrentCamera() ), renderables["SecurityCamera"] );
	renderables["SecurityCamera"].SetRotation( 0.0f, rotation, 0.0f );

	// Update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}

void Level1::CleanUp()
{
	renderables.clear();
	cubes.clear();
}
