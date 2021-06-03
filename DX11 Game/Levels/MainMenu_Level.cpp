#include "stdafx.h"
#include "MainMenu_Level.h"
#include "Collisions.h"
#include "Rasterizer.h"

MainMenu_Level::MainMenu_Level( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool MainMenu_Level::OnCreate()
{
	try
	{
		// User Interface
		Menu = make_shared<Main_Menu_UI>();
		settingsUi = make_shared<Settings_Menu_UI>();
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void MainMenu_Level::OnSwitch()
{
	// Update Level System
	levelName = "MainMenu";
	numOfCubes = 0;
	LevelContainer::UpdateCubes();

	EventSystem::Instance()->AddEvent( EVENTID::ShowCursorEvent );

	// Update UI System
	_UiManager->RemoveAllUI();
	_UiManager->AddUi( Menu, "MainMenu" );
	_UiManager->AddUi( settingsUi, "Settings" );
	_UiManager->Initialize( graphics->device.Get(), graphics->context.Get(), &cb_vs_matrix_2d );

	// Update Sound System
	Sound::Instance()->InitialiseSoundEffect( "MenuClick" );
	Sound::Instance()->InitialiseMusicTrack( "MenuMusic" );

	Sound::Instance()->PlayMusic( "MenuMusic" );
}

void MainMenu_Level::Render()
{
	// Render to sub viewport first using static camera
	GetMultiViewport()->SetUsingSub();
	BeginFrame();
	//RenderFrame(); // don't render security camera view

	// Render main scene next with main/debug camera
	GetMultiViewport()->SetUsingMain();
	BeginFrame();
	RenderFrame();

	// Render UI and present the complete frame
	EndFrame();
}

void MainMenu_Level::RenderFrame()
{
	// Render lights/skysphere
	LevelContainer::RenderFrameEarly();
}

void MainMenu_Level::Update( const float dt )
{
	// Update lights/skysphere
	LevelContainer::Update( dt );

	// Update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}

void MainMenu_Level::CleanUp()
{
}
