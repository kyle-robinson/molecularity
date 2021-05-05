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
	cameras.Update();
	gfx.Update( dt );
	//update screen size
	RECT windowRect;
	if (GetClientRect(renderWindow.GetHWND(), &windowRect)) {

		XMFLOAT2 windowsize = { (float)(windowRect.right - windowRect.left),(float)(windowRect.bottom - windowRect.top) };
		EventSystem::Instance()->AddEvent(EVENTID::WindowSizeChangeEvent, &windowsize);
		cameras.UpDateUICamValues(windowsize.x, windowsize.y, 0.0f, 1.0f);
		gfx.UpdateWidthHight(windowsize.x, windowsize.y);
		renderWindow.SetWidthHight(windowsize.x, windowsize.y);
	}

	////full screen
	//https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
	//WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
	//DWORD dwStyle = GetWindowLong(renderWindow.GetHWND(), GWL_STYLE);
	//if (dwStyle & WS_OVERLAPPEDWINDOW) {
	//	MONITORINFO mi = { sizeof(mi) };
	//	if (GetWindowPlacement(renderWindow.GetHWND(), &g_wpPrev) &&
	//		GetMonitorInfo(MonitorFromWindow(renderWindow.GetHWND(),
	//			MONITOR_DEFAULTTOPRIMARY), &mi)) {
	//		SetWindowLong(renderWindow.GetHWND(), GWL_STYLE,
	//			dwStyle & ~WS_OVERLAPPEDWINDOW);
	//		SetWindowPos(renderWindow.GetHWND(), HWND_TOP,
	//			mi.rcMonitor.left, mi.rcMonitor.top,
	//			mi.rcMonitor.right - mi.rcMonitor.left,
	//			mi.rcMonitor.bottom - mi.rcMonitor.top,
	//			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	//	}
	//}

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