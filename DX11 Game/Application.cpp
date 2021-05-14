#include "Application.h"
#include "CameraMovement.h"
#include "Utility/EventSystem/EventSystem.h"
#include <thread>

bool Application::Initialize(
	HINSTANCE hInstance,
	const std::string& windowTitle,
	const std::string& windowClass,
	int width,
	int height )
{
	// initialize delta time
	timer.Start();

	// GRAPHICS
	{
		// initialize graphics
		if ( !renderWindow.Initialize( &input, hInstance, windowTitle, windowClass, width, height ) ) return false;
		if ( !gfx.Initialize( renderWindow.GetHWND(), width, height ) ) return false;
		imgui.Initialize( renderWindow.GetHWND(), gfx.device.Get(), gfx.context.Get() );
	}

	// LEVELS
	{
		// initialize levels
		level1 = std::make_shared<Level1>( stateMachine );
		std::thread first( &Level1::Initialize, level1, &gfx, &cameras, &imgui, &_UI_Manager, &sound );
		level1.get()->SetTool(&tool);
		first.join();

		level2 = std::make_shared<Level2>( stateMachine );
		std::thread second( &Level2::Initialize, level2, &gfx, &cameras, &imgui, &_UI_Manager, &sound );
		level2.get()->SetTool(&tool);
		second.join();

		//main menu
		MainMenu = std::make_shared<MainMenu_Level>( stateMachine );
		std::thread third( &MainMenu_Level::Initialize, MainMenu, &gfx, &cameras, &imgui, &_UI_Manager, &sound );
		MainMenu.get()->SetTool(&tool);
		third.join();

		// add levels to state machine
		level1_ID = stateMachine.Add( level1 );
		level2_ID = stateMachine.Add( level2 );
		MainMenu_ID = stateMachine.Add( MainMenu );

		stateMachine.SwitchTo( MainMenu_ID );
	}

	// SYSTEMS
	{
		// initialize cameras
		cameras.Initialize( width, height );

		// add levels to list
		std::vector<uint32_t> level_IDs;
		level_IDs.push_back( std::move( level1_ID ) );
		level_IDs.push_back( std::move( level2_ID ) );
		level_IDs.push_back( std::move( MainMenu_ID ) );
    
		// initialize input
		input.Initialize( renderWindow, &stateMachine, &cameras, &sound, level_IDs );
	}

	//load settings
	_SettingsData = JSON::LoadSettings();
	EventSystem::Instance()->AddEvent( EVENTID::UpdateSettingsEvent, &_SettingsData );


	//Process Initialize events 
	EventSystem::Instance()->ProcessEvents();
	return true;
}

bool Application::ProcessMessages() noexcept
{
	return renderWindow.ProcessMessages();
}

void Application::Update()
{
	// delta time
	float dt = static_cast< float >( timer.GetMilliSecondsElapsed() );
	timer.Restart();

	// update systems
	input.Update( dt );
	sound.UpdatePosition( cameras.GetCamera( cameras.GetCurrentCamera() )->GetPositionFloat3(), cameras.GetCamera( cameras.GetCurrentCamera() )->GetRotationFloat3().y ); // Update to make this every few frames
	cameras.Update();

	// update current level
	stateMachine.Update( dt );
	EventSystem::Instance()->ProcessEvents();
}

void Application::Render()
{
	// render current level
	stateMachine.Render();
}