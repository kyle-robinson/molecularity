#include "Application.h"
#include "CameraMovement.h"
#include "Utility/EventSystem/EventSystem.h"

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
	if ( !gfx.Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;

	// input
	cameras.Initialize( width, height );
	input.Initialize( &gfx, renderWindow, &cameras, width, height );

	// sound
	if (FAILED(sound.PlayMusic(sound.MUSIC_MAIN, true))) return false;

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
	sound.UpdatePosition(cameras.GetCamera(cameras.GetCurrentCamera())->GetPositionFloat3(), cameras.GetCamera(cameras.GetCurrentCamera())->GetRotationFloat3().y); // Update to make this every few frames
	gfx.Update( dt );
	EventSystem::Instance()->ProcessEvents();
}

void Application::Render()
{
	// Render to sub viewport first using static camera
	gfx.GetMultiViewport()->SetUsingSub( true );
	gfx.BeginFrame();
	gfx.RenderFrame();

	// Render main scene next with main/debug camera
	gfx.GetMultiViewport()->SetUsingMain( true );
	gfx.BeginFrame();
	gfx.RenderFrame();

	// Render UI and present the complete frame
	gfx.EndFrame();
}