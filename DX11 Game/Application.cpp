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
		std::thread first( &Level1::Initialize, level1, &gfx, &cameras, &imgui );
		first.join();

		level2 = std::make_shared<Level2>( stateMachine );
		std::thread second( &Level2::Initialize, level2, &gfx, &cameras, &imgui );
		second.join();

		// add levels to state machine
		level1_ID = stateMachine.Add( level1 );
		level2_ID = stateMachine.Add( level2 );
		stateMachine.SwitchTo( level1_ID );
	}

	// SYSTEMS
	{
		// initialize sound
		sound.SetMusicVolume( 0.5f );
		if ( FAILED( sound.PlayMusic( sound.MUSIC_MAIN, true ) ) ) return false;

		// initialize cameras
		cameras.Initialize( width, height );

		// add levels to list
		std::vector<uint32_t> level_IDs;
		level_IDs.push_back( std::move( level1_ID ) );
		level_IDs.push_back( std::move( level2_ID ) );

		// initialize input
		input.Initialize( renderWindow, &stateMachine, &cameras, &sound, level_IDs );
	}

	return true;
}

bool Application::ProcessMessages() noexcept
{
	return renderWindow.ProcessMessages();
}

void Application::Update()
{
	// delta time
	float dt = static_cast<float>( timer.GetMilliSecondsElapsed() );
	timer.Restart();

	// update systems
	input.Update( dt );
	sound.UpdatePosition(
		cameras.GetCamera( cameras.GetCurrentCamera() )->GetPositionFloat3(),
		cameras.GetCamera( cameras.GetCurrentCamera() )->GetRotationFloat3().y
	); // update to make this every few frames
	cameras.Update();

	// update screen size
	RECT windowRect;
	if ( GetClientRect( renderWindow.GetHWND(), &windowRect ) ) {

		XMFLOAT2 windowsize = { ( float )( windowRect.right - windowRect.left ), ( float )( windowRect.bottom - windowRect.top ) };
		EventSystem::Instance()->AddEvent( EVENTID::WindowSizeChangeEvent, &windowsize );
	}

	// update current level
	stateMachine.Update( dt );
	EventSystem::Instance()->ProcessEvents();
}

void Application::Render()
{
	// render current level
	stateMachine.Render();
}