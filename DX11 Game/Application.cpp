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
	cameras.Initialize(width, height);
	if ( !renderWindow.Initialize( &input, hInstance, windowTitle, windowClass, width, height ) ) return false;
	if ( !gfx.Initialize( renderWindow.GetHWND(),&cameras, width, height ) ) return false;
	input.Initialize( &gfx, renderWindow,&cameras, width, height);

	if (!sound.Initialise(renderWindow.GetHWND()))
	{
		MessageBox(renderWindow.GetHWND(), L"Could not initialise Direct Sound.", L"Error", MB_OK);
		return false;
	}

	sound.PlayWavFile(MAIN_MUSIC, 0.75f);

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
	gfx.BeginFrame();
	gfx.RenderFrame();
	gfx.EndFrame();
}