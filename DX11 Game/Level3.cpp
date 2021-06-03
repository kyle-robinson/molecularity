#include "stdafx.h"
#include "Level3.h"
#include "Billboard.h"
#include "Collisions.h"
#include "Rasterizer.h"
#include "PostProcessing.h"
#include "ModelData.h"

Level3::Level3( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool Level3::OnCreate()
{
	try
	{
		// DRAWABLES
		if ( !ModelData::LoadModelData( "Level3_Objects.json" ) ) return false;
		if ( !ModelData::InitializeModelData( *&graphics->context, *&graphics->device, cb_vs_matrix, renderables ) ) return false;

		// UI
		HUD = make_shared<HUD_UI>();
		PauseUI = make_shared<Pause>();
		EndLevelUI = make_shared<EndLevelScreen_UI>();

		// CIRCUIT POINTS
		brokenCircuitPoints.push_back( std::make_pair( XMFLOAT3( -6.0f, 0.0f, 10.0f ), false ) );
		brokenCircuitPoints.push_back( std::make_pair( XMFLOAT3( 7.5f, 0.0f, 34.5f ), false ) );
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
	numOfCubes = 3;
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
	Sound::Instance()->InitialiseSoundEffect( "LevelComplete" );
	Sound::Instance()->InitialiseSoundEffect( "MenuClick" );
	Sound::Instance()->InitialiseSoundEffect( "PoweredOn" );

	Sound::Instance()->PlayMusic( "LevelMusic" );

	// Initialize Camera Positions
	cameras->GetCamera( JSON::CameraType::Default )->SetInitialPosition( 0.0f, 7.0f, -20.0f );
	cameras->GetCamera( JSON::CameraType::Static )->SetInitialPosition( 0.0f, 10.0f, 65.0f );
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

	// draw w/ back-face culling
	graphics->GetRasterizer( "Skybox" )->Bind( *graphics );
	renderables["Room"].Draw();
	graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );

	// draw with back-face culling
	if ( renderDoor )
		renderables["Door"].Draw();

	renderables["PressurePlate"].Draw();
	renderables["SecurityCamera"].Draw();

	// render the cubes
	LevelContainer::RenderFrame();
}

void Level3::Update( const float dt )
{
	// update lights/skysphere
	LevelContainer::Update( dt );

	// camera collisions w room
	Collisions::CheckCollisionLevel3( cameras->GetCamera( JSON::CameraType::Default ), 16.5f );

	// cube collisions
	for ( uint32_t i = 0; i < numOfCubes; i++ )
	{
		// update collisions w other cubes
		for ( uint32_t j = 0; j < numOfCubes; j++ ) if ( i != j )
			cubes[i]->CheckCollisionAABB( cubes[j], dt );

		// update collisions w room
		Collisions::CheckCollisionLevel3( cubes[i], 17.0f );

		// range check with broken circuit points
		for ( uint32_t j = 0; j < brokenCircuitPoints.size(); j++ )
		{
			// get if cube is in range
			bool isInRange = Collisions::CheckCollisionSphere( brokenCircuitPoints[j].first, *cubes[i], 5.0f );

			// activate circuit point if cube is in range and magnetic
			if ( cubes[i]->GetEditableProperties()->GetConductivity() && isInRange )
			{
				brokenCircuitPoints[j].second = true;

				// need a better way to do this - currently only way to fix sound looping
				static bool playOnce1 = true;
				if ( j == 0 && playOnce1 )
				{
					Sound::Instance()->PlaySoundEffect( "PoweredOn" );
					playOnce1 = false;
				}
				static bool playOnce2 = true;
				if ( j == 1 && playOnce2 )
				{
					Sound::Instance()->PlaySoundEffect( "PoweredOn" );
					playOnce2 = false;
				}
			}
		}

		// update collisions w pressure plate
		if ( cubes[i]->CheckCollisionAABB( renderables["PressurePlate"], dt ) )
		{
			// check if the pressure plate is powered
			bool hasPower = true;
			for ( uint32_t j = 0; j < brokenCircuitPoints.size(); j++ )
				if ( !brokenCircuitPoints[j].second )
					hasPower = false;

			// activate the plate if it has power
			if ( cubes[i]->GetPhysicsModel()->GetMass() > 100.0f && !levelCompleted && hasPower )
			{
				levelCompleted = true;
				Sound::Instance()->PlaySoundEffect( "LevelComplete" );
			}
		}
	}

	// update postProcessing
	if ( !brokenCircuitPoints[0].second && !brokenCircuitPoints[1].second )
		postProcessing->BindMonochrome();

	if ( ( brokenCircuitPoints[0].second && !brokenCircuitPoints[1].second ) ||
		( !brokenCircuitPoints[0].second && brokenCircuitPoints[1].second ) )
		postProcessing->BindSepia();

	if ( brokenCircuitPoints[0].second && brokenCircuitPoints[1].second )
		postProcessing->UnbindEffect();

	// update door if first plate is active
	if ( brokenCircuitPoints[0].second )
		renderDoor = false;

	// set rotation of security camera
	float rotation = Billboard::BillboardModel( cameras->GetCamera( cameras->GetCurrentCamera() ), renderables["SecurityCamera"] );
	renderables["SecurityCamera"].SetRotation( 0.0f, rotation, 0.0f );

	// update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}