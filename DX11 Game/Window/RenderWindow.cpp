#include "WindowContainer.h"
#include "Resources\\Resource.h"
#include <JSON_Helper.h>

// Window icon made by https://www.freepik.com Freepik from https://www.flaticon.com/

bool RenderWindow::Initialize( WindowContainer* pWindowContainer, HINSTANCE hInstance,
	const std::string& windowName, const std::string& windowClass, int width, int height )
{	
	AddToEvent();
	// register window class
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	windowTitle = windowName;
	windowTitle_Wide = StringConverter::StringToWide( windowName );
	this->windowClass = windowClass;
	windowClass_Wide = StringConverter::StringToWide( windowClass );

	hCursorNormal = LoadCursor( hInstance, IDC_ARROW );
	hCursorNightNormal = LoadCursor( hInstance, (LPCWSTR)IDR_ANICURSOR1 );
	hCursorNightSelect = LoadCursor( hInstance, (LPCWSTR)IDR_ANICURSOR2 );

	RegisterWindowClass();

	int centerScreenX = GetSystemMetrics( SM_CXSCREEN ) / 2 - width / 2;
	int centerScreenY = GetSystemMetrics( SM_CYSCREEN ) / 2 - height / 2;

	RECT windowRect;
	windowRect.left = centerScreenX;
	windowRect.top = centerScreenY;
	windowRect.right = windowRect.left + width;
	windowRect.bottom = windowRect.top + height;
	AdjustWindowRect( &windowRect, WS_OVERLAPPEDWINDOW, FALSE );

	// create window
	hWnd = CreateWindow(
		windowClass_Wide.c_str(),
		windowTitle_Wide.c_str(),
		WS_OVERLAPPEDWINDOW,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		pWindowContainer
	);

	if ( hWnd == NULL )
	{
		ErrorLogger::Log( GetLastError(), "ERROR::CreateWindow Failed for window: " + windowTitle );
		return false;
	}

	// show window
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );

	return true;
}

LRESULT CALLBACK HandleMsgRedirect( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch ( uMsg )
	{
	case WM_CLOSE:
		DestroyWindow( hWnd );
		return 0;

	// handle all other messages
	default:
	{
		// get ptr to window class
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
		// forward messages to window class
		return pWindow->WindowProc( hWnd, uMsg, wParam, lParam );
	}
	}
}

LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch ( uMsg )
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>( pCreate->lpCreateParams );
		if ( pWindow == nullptr )
		{
			ErrorLogger::Log( "ERROR::Pointer to window container is null during WM_NCCREATE!" );
			exit( -1 );
		}
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWindow ) );
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( HandleMsgRedirect ) );
		return pWindow->WindowProc( hWnd, uMsg, wParam, lParam );
	}

	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}

bool RenderWindow::ProcessMessages() noexcept
{
	MSG msg = { 0 };
	while ( PeekMessage( &msg, hWnd, 0u, 0u, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// check if the window was closed
	if ( msg.message == WM_NULL )
	{
		if ( !IsWindow( hWnd ) )
		{
			hWnd = NULL;
			UnregisterClass( windowClass_Wide.c_str(), hInstance );
			return false;
		}
	}

	return true;
}

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
void RenderWindow::AddToEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::QuitGameEvent, this);
}
static int width, hight;
void RenderWindow::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::QuitGameEvent:
	{
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		
	}
	break;
	case EVENTID::UpdateSettingsEvent:
	{
		//full screen
		//change in window size
		
		std::vector<JSON::SettingData> a = *static_cast<std::vector<JSON::SettingData>*>(event->GetData());
		for (auto& setting : a)
		{
			SetIsStopNextFrame(true);
			if (setting.Name == "FullScreen") {
				//setfullscreen
				WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
				DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
					MONITORINFO mi = { sizeof(mi) };
				if (bool* input = std::get_if<bool>(&setting.Setting)) {
					if (*input) {
					
						if (dwStyle & WS_OVERLAPPEDWINDOW) {
							
							if (GetWindowPlacement(hWnd, &g_wpPrev) &&
								GetMonitorInfo(MonitorFromWindow(hWnd,
									MONITOR_DEFAULTTOPRIMARY), &mi)) {
								SetWindowLong(hWnd, GWL_STYLE,
									dwStyle & ~WS_OVERLAPPEDWINDOW);
								SetWindowPos(hWnd, HWND_TOP,
									mi.rcMonitor.left, mi.rcMonitor.top,
									mi.rcMonitor.right - mi.rcMonitor.left,
									mi.rcMonitor.bottom - mi.rcMonitor.top,
									SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
								
							}
						}
					}
					else
					{
						GetMonitorInfo(MonitorFromWindow(hWnd,
							MONITOR_DEFAULTTOPRIMARY), &mi);

						SetWindowLong(hWnd, GWL_STYLE,
							dwStyle | WS_OVERLAPPEDWINDOW);
						SetWindowPlacement(hWnd, &g_wpPrev);
						SetWindowPos(hWnd, NULL, ((mi.rcMonitor.right - mi.rcMonitor.left)/2)- width/2,
							((mi.rcMonitor.bottom - mi.rcMonitor.top)/2)- hight/2, width, hight, SWP_SHOWWINDOW |  SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
					}
					
					SetCapture(hWnd);
					POINT pt;
					pt.x = width/2;
					pt.y = 0;
					ClientToScreen(hWnd, &pt);
					SetCursorPos(pt.x, pt.y);
				
					
				}
				
			}
			else if (setting.Name == "WindowWidth") {
				width = std::get<int>(setting.Setting);
			}
			else if (setting.Name == "WindowHight") {
				hight = std::get<int>(setting.Setting);
			}
		}

	}
		break;
	case EVENTID::WindowSizeChangeEvent:
		SetIsStopNextFrame(true);
		DirectX::XMFLOAT2 _SizeOfScreen = *static_cast<DirectX::XMFLOAT2*>(event->GetData());
		width = _SizeOfScreen.x;
		height = _SizeOfScreen.y;
		break;
	
	}
}

void RenderWindow::RegisterWindowClass() noexcept
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof( wc );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( hInstance, (LPCTSTR)IDR_ICON1 );
    wc.hCursor = hCursorNightNormal;
    wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClass_Wide.c_str();
	wc.hIconSm = LoadIcon( wc.hInstance, (LPCTSTR)IDR_ICON1 );
	RegisterClassEx( &wc );
}

RenderWindow::~RenderWindow() noexcept
{
	if ( hWnd != NULL )
	{
		UnregisterClass( windowClass_Wide.c_str(), hInstance );
		DestroyWindow( hWnd );
	}
}