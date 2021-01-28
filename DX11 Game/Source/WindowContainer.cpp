#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
	static bool rawInputInitialized = false;
	if ( !rawInputInitialized )
	{
		RAWINPUTDEVICE rid = { 0 };
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if ( RegisterRawInputDevices( &rid, 1, sizeof( rid ) ) == FALSE )
		{
			ErrorLogger::Log( GetLastError(), "Failed to register raw input devices!" );
			exit( -1 );
		}

		rawInputInitialized = true;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK WindowContainer::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
		return true;

	switch ( uMsg )
	{
	case WM_PAINT:
		HDC hdc;
		PAINTSTRUCT ps;
        hdc = BeginPaint( hWnd, &ps );
        EndPaint( hWnd, &ps );
		return 0;
    case WM_DESTROY:
		DestroyWindow( hWnd );
		PostQuitMessage( 0 );
		exit( -1 );
		return 0;
		// Keyboard Events
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>( wParam );
		if ( keyboard.IsKeysAutoRepeat() )
			keyboard.OnKeyPressed( keycode );
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if ( !wasPressed )
				keyboard.OnKeyPressed( keycode );
		}
		switch ( wParam )
		{
		case VK_ESCAPE:
			DestroyWindow( renderWindow.GetHWND() );
			PostQuitMessage( 0 );
			return 0;
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>( wParam );
		keyboard.OnKeyReleased( keycode );
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>( wParam );
		if ( keyboard.IsCharsAutoRepeat() )
			keyboard.OnChar( ch );
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if ( !wasPressed )
				keyboard.OnChar( ch );
		}
		return 0;
	}
	// Mouse Events
	case WM_MOUSEMOVE:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnMouseMove( x, y );
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnLeftPressed( x, y );
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnLeftReleased( x, y );
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnRightPressed( x, y );
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnRightReleased( x, y );
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnMiddlePressed( x, y );
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		mouse.OnMiddleReleased( x, y );
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD( lParam );
		int y = HIWORD( lParam );
		if ( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
		{
			mouse.OnWheelUp( x, y );
		}
		else if ( GET_WHEEL_DELTA_WPARAM( wParam ) < 0 )
		{
			mouse.OnWheelDown( x, y );
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT dataSize;
		GetRawInputData( reinterpret_cast<HRAWINPUT>( lParam ), RID_INPUT, NULL, &dataSize, sizeof( RAWINPUTHEADER ) );
		if ( dataSize > 0 )
		{
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>( dataSize );
			if ( GetRawInputData( reinterpret_cast<HRAWINPUT>( lParam ), RID_INPUT, rawData.get(), &dataSize, sizeof( RAWINPUTHEADER ) ) == dataSize )
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>( rawData.get() );
				if ( raw->header.dwType == RIM_TYPEMOUSE )
				{
					mouse.OnMouseMoveRaw( raw->data.mouse.lLastX, raw->data.mouse.lLastY );
				}
			}
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}