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
	if ( !gfx.Initialize( renderWindow.GetHWND(), &cameras, width, height ) ) return false;

	// input
	cameras.Initialize( width, height );
	input.Initialize( &gfx, renderWindow, &cameras, width, height );

	// sound
	if ( !sound.Initialize( renderWindow.GetHWND() ) ) return false;
	if ( !sound.PlayWavFile( MAIN_MUSIC, 0.75f, { 0.0f, 0.0f, 0.0f } ) ) return false;

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
	gfx.Update( dt );
	
	//update width/hight items for when window size changes
	RECT rect;
	if (GetClientRect(renderWindow.GetHWND(), &rect))
	{
		
		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		renderWindow.SetWidthHight(width, height);
		gfx.UpdateWidthHight(width, height);
		cameras.GetCamera(cameras.GetCurrentCamera())->SetProjectionValues(70.0f, width / height, 0.1f, 1000.0f);
		cameras.UpDateUICamValues(width, height, 0.0f, 1.0f);
		
	}
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