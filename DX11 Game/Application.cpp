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

	if ( !renderWindow.Initialize( &input, hInstance, windowTitle, windowClass, width, height ) )
		return false;
	
	if ( !graphics.Initialize( renderWindow.GetHWND(), width, height ) )
		return false;

	input.Initialize(&graphics,&renderWindow, width, height);


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
	input.Update(dt);
	graphics.Update( dt );
}

void Application::Render()
{
	graphics.BeginFrame();
	graphics.RenderFrame();
	graphics.EndFrame();
}