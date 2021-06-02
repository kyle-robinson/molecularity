#include "stdafx.h"
#include "Level3.h"
#include "Billboard.h"
#include "Collisions.h"
#include "Rasterizer.h"
#include "PostProcessing.h"

Level3::Level3( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool Level3::OnCreate()
{
	try
	{
		// DRAWABLES
		if ( !room.Initialize( "Resources\\Models\\Levels\\Level3-V1.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
		room.SetInitialScale( 0.005f, 0.005f, 0.005f );
		room.SetInitialPosition( 0.5f, 0.0f, -17.5f );
		room.SetInitialRotation( 0.0f, XM_PI, 0.0f );

		if ( !door.Initialize( "Resources\\Models\\Levels\\Door.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
		door.SetInitialScale( 0.005f, 0.005f, 0.005f );
		door.SetInitialPosition( 0.5f, 0.0f, -17.5f );
		door.SetInitialRotation( 0.0f, XM_PI, 0.0f );

		if ( !pressurePlate.Initialize( "Resources\\Models\\PressurePlate.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
		pressurePlate.SetInitialPosition( 0.0f, 0.0f, 57.0f );
		pressurePlate.SetInitialScale( 0.025f, 0.025f, 0.025f );

		if ( !securityCamera.Initialize( "Resources\\Models\\Camera\\scene.gltf", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
		securityCamera.SetInitialScale( 2.0f, 2.0f, 2.0f );
		securityCamera.SetInitialPosition( 0.0f, 10.0f, 67.5f );

		// UI
		HUD = make_shared<HUD_UI>();
		PauseUI = make_shared<Pause>();
		EndLevelUI = make_shared<EndLevelScreen_UI>();

		// CIRCUIT POINTS
		brokenCircuitPoints.push_back( std::make_pair( XMFLOAT3( -6.0f, 0.0f, 10.0f ), false ) );
		brokenCircuitPoints.push_back( std::make_pair( XMFLOAT3( 7.5f, 0.0f, 34.5f ), false ) );

		// POST-PROCESSING
		//postProcessing->BindMonochrome();
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
	Sound::Instance()->PlaySoundEffect( "LevelComplete" );
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
	room.Draw();
	graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );

	// draw with back-face culling
	if ( renderDoor )
		door.Draw();
	pressurePlate.Draw();
	securityCamera.Draw();

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
				Sound::Instance()->PlaySoundEffect( "PoweredOn" );
			}
		}

		// update collisions w pressure plate
		if ( cubes[i]->CheckCollisionAABB( pressurePlate, dt ) )
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
	float rotation = Billboard::BillboardModel( cameras->GetCamera( cameras->GetCurrentCamera() ), securityCamera );
	securityCamera.SetRotation( 0.0f, rotation, 0.0f );

	// update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}