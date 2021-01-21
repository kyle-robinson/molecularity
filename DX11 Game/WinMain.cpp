#include "stdafx.h"
#include "Application.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    std::unique_ptr<Application> app = std::make_unique<Application>();
    if ( !( app->Initialise() ) ) return -1;

    MSG msg = { 0 };
    while ( WM_QUIT != msg.message )
    {
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            bool handled = false;

			if ( msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST )
				handled = app->HandleKeyboard( msg );
			else if ( WM_QUIT == msg.message )
				break;

			if ( !handled )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
        }
        else
        {
            app->Update();
            app->Draw();
        }
    }

    return static_cast<int>( msg.wParam );
}