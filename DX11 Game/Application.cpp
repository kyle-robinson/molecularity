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
		std::thread first( &Level1::Initialize, level1, &gfx, &cameras, &imgui, &_UI_Manager );
		level1.get()->SetTool( &tool );
		first.join();

		level2 = std::make_shared<Level2>( stateMachine );
		std::thread second( &Level2::Initialize, level2, &gfx, &cameras, &imgui, &_UI_Manager );
		level2.get()->SetTool( &tool );
		second.join();

		level3 = std::make_shared<Level3>( stateMachine );
		std::thread third( &Level3::Initialize, level3, &gfx, &cameras, &imgui, &_UI_Manager );
		level3.get()->SetTool( &tool );
		third.join();

		//main menu
		MainMenu = std::make_shared<MainMenu_Level>( stateMachine );
		std::thread fourth( &MainMenu_Level::Initialize, MainMenu, &gfx, &cameras, &imgui, &_UI_Manager );
		MainMenu.get()->SetTool( &tool );
		fourth.join();

		//credits
		Credits = std::make_shared<Credits_Level>( stateMachine );
		std::thread fifth( &Credits_Level::Initialize, Credits, &gfx, &cameras, &imgui, &_UI_Manager );
		Credits.get()->SetTool( &tool );
		fifth.join();

		// add levels to state machine
		MainMenu_ID = stateMachine.Add( MainMenu );
		level1_ID = stateMachine.Add( level1 );
		level2_ID = stateMachine.Add( level2 );
		level3_ID = stateMachine.Add( level3 );
		Credits_ID = stateMachine.Add( Credits );

		stateMachine.SwitchTo( MainMenu_ID );

		// initialize level inputs
		input.Initialize( renderWindow, &stateMachine, &cameras );
	}

	// SYSTEMS
	{
		cameras.Initialize( width, height );

		//load settings
		_SettingsData = JSON::LoadSettings();
		EventSystem::Instance()->AddEvent( EVENTID::UpdateSettingsEvent, &_SettingsData );

		//Process Initialize events 
		EventSystem::Instance()->ProcessEvents();
	}

	return true;
}

bool Application::ProcessMessages() noexcept
{
	return renderWindow.ProcessMessages();
}

void Application::Update()
{	

	if ( renderWindow.GetIsStopNextFrame() )
	{
		// delta time
		float dt = static_cast<float>( timer.GetMilliSecondsElapsed() );
		timer.Restart();

		// update systems
		input.Update( dt );
		cameras.Update();

		// update current level
		stateMachine.Update( dt );
		EventSystem::Instance()->ProcessEvents();
	}
	else
	{
		renderWindow.SetIsStopNextFrame( false );
	}
}

void Application::Render()
{
	// render current level
	stateMachine.Render();
}