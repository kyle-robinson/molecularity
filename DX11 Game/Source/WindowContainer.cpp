#include "stdafx.h"
#include "WindowContainer.h"

WindowContainer::WindowContainer() { }

LRESULT CALLBACK WindowContainer::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
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
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}