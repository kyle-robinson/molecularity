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

	if ( !renderWindow.Initialize( &input, hInstance, windowTitle, windowClass, width, height ) ) return false;
	if ( !gfx.Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;

	cameras.Initialize( width, height );
	input.Initialize( &gfx, renderWindow, &cameras, width, height);

	//if ( !sound.Initialize( renderWindow.GetHWND() ) ) return false;
	//if ( !sound.PlayWavFile( MAIN_MUSIC, 0.75f ) ) return false;

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
	input.Update( dt );
	gfx.Update( dt );
}

void Application::Render()
{
	gfx.useViewportSubWrite = true;
	gfx.BeginFrame();
	gfx.RenderFrame();

	gfx.useViewportSubDraw = true;
	gfx.BeginFrame();
	gfx.RenderFrame();

	gfx.useViewportMain = true;
	gfx.BeginFrame();
	gfx.RenderFrame();

	gfx.EndFrame();
}