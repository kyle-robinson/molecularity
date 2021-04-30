#include "Application.h"
#include "CameraMovement.h"

bool Application::Initialize(
	HINSTANCE hInstance,
	const std::string& windowTitle,
	const std::string& windowClass,
	int width,
	int height )
{
	// initialize delta time
	timer.Start();

	// initialize graphics
	if ( !renderWindow.Initialize( &input, hInstance, windowTitle, windowClass, width, height ) ) return false;
	if ( !gfx.Initialize( renderWindow.GetHWND(), width, height ) ) return false;
	imgui.Initialize( renderWindow.GetHWND(), gfx.device.Get(), gfx.context.Get() );

	// initialize levels
	level1 = std::make_shared<Level1>( stateMachine );
	if ( !level1->Initialize( &gfx, &cameras, &imgui ) ) return false;

	//level2 = std::make_shared<Level2>( stateMachine );
	//if ( !level2->Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;

	// add levels to state machine
	level1_ID = stateMachine.Add( level1 );
	//level2_ID = stateMachine.Add( level2 );
	stateMachine.SwitchTo( level1_ID );

	// initialize input
	cameras.Initialize( width, height );
	input.Initialize( level1.get(), renderWindow, &cameras, width, height );

	// initialize sound
	if ( !sound.Initialize( renderWindow.GetHWND() ) ) return false;
	if ( !sound.PlayWavFile( sound.MAIN_MUSIC, 0.75f ) ) return false;

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
	sound.UpdateListenerPos( ( cameras.GetCamera( cameras.GetCurrentCamera() )->GetPositionFloat3() ) );
	input.Update( dt, sound );

	// update current level
	stateMachine.Update( dt );
	if ( GetAsyncKeyState( 'J' ) )
		stateMachine.SwitchTo( level1_ID );
	//if ( GetAsyncKeyState( 'K' ) )
	//	stateMachine.SwitchTo( level2_ID );
}

void Application::Render()
{
	// render current level
	stateMachine.Render();
}