#include "stdafx.h"
#include "MainMenu_Level.h"
#include "Collisions.h"
#include "Rasterizer.h"

MainMenu_Level::MainMenu_Level( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool MainMenu_Level::OnCreate()
{
	try
	{
		// UI
		{
			Menu = make_shared<Main_Menu_UI>();
			settingsUi = make_shared<Settings_Menu_UI>();
		}
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
	// update items on level switch here...
	levelName = "MainMenu";
	numOfCubes = 0;
	LevelContainer::UpdateCubes();

	//make sure cursor is displayed
	EventSystem::Instance()->AddEvent( EVENTID::ShowCursorEvent );

	//UI
	_UiManager->RemoveAllUI();
	_UiManager->AddUi( Menu, "MainMenu" );
	_UiManager->AddUi( settingsUi, "Settings" );
	_UiManager->Initialize( graphics->device.Get(), graphics->context.Get(), &cb_vs_matrix_2d );

	//sounds
	Sound::Instance()->InitialiseSoundEffect( "MenuClick" );
	Sound::Instance()->InitialiseMusicTrack( "MenuMusic" );
	Sound::Instance()->PlayMusic( "MenuMusic" );
}

void MainMenu_Level::Render()
{
	// Render to sub viewport first using static camera
	GetMultiViewport()->SetUsingSub();
	BeginFrame();
	//RenderFrame();

	// Render main scene next with main/debug camera
	GetMultiViewport()->SetUsingMain();
	BeginFrame();
	RenderFrame();

	// Render UI and present the complete frame
	EndFrame();
}

void MainMenu_Level::RenderFrame()
{
	// render ligths/skysphere
	LevelContainer::RenderFrameEarly();
}

void MainMenu_Level::Update( const float dt )
{
	// update lights/skysphere
	LevelContainer::Update( dt );

	// update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}