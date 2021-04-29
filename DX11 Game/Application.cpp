#include "Application.h"
#include "CameraMovement.h"

bool Application::Initialize(
	HINSTANCE hInstance,
	const std::string& windowTitle,
	const std::string& windowClass,
	int width,
	int height )
{
	timer.Start();


	// graphics
	if ( !renderWindow.Initialize( &input, hInstance, windowTitle, windowClass, width, height ) ) return false;
	//if ( !levelManager.Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;
	level1 = std::make_shared<Level1>( stateMachine );
	if ( !level1->Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;
	level2 = std::make_shared<Level2>( stateMachine );
	if ( !level2->Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;
	//if ( !level1.OnCreate() ) return false;

	level1_ID = stateMachine.Add( level1 );
	level2_ID = stateMachine.Add( level2 );
	stateMachine.SwitchTo( level1_ID );

	// input
	cameras.Initialize( width, height );
	input.Initialize( level1.get(), renderWindow, &cameras, width, height );

	// sound
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
	float dt = static_cast<float>( timer.GetMilliSecondsElapsed() );
	timer.Restart();
	sound.UpdateListenerPos( ( cameras.GetCamera( cameras.GetCurrentCamera() )->GetPositionFloat3() ) );
	input.Update( dt, sound );

	stateMachine.Update( dt );
	if ( GetAsyncKeyState( 'J' ) )
		stateMachine.SwitchTo( level1_ID );
	if ( GetAsyncKeyState( 'K' ) )
		stateMachine.SwitchTo( level2_ID );
}

void Application::Render()
{
	/*if ( !usingLevel2 )
	{
		// Render to sub viewport first using static camera
		level1->GetMultiViewport()->SetUsingSub( true );
		level1->BeginFrame();
		level1->RenderFrame();

		// Render main scene next with main/debug camera
		level1->GetMultiViewport()->SetUsingMain( true );
		level1->BeginFrame();
		level1->RenderFrame();

		// Render UI and present the complete frame
		level1->EndFrame();
	}
	else
	{
	}
	// Render to sub viewport first using static camera
	level2->GetMultiViewport()->SetUsingSub( true );
	level2->BeginFrame();
	level2->RenderFrame();

	// Render main scene next with main/debug camera
	level2->GetMultiViewport()->SetUsingMain( true );
	level2->BeginFrame();
	level2->RenderFrame();*/

	// Render UI and present the complete frame
	stateMachine.RenderFrame();
}