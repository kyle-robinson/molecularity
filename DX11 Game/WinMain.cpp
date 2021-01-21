#include "stdafx.h"
#include "Application.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow );

    HRESULT hr = CoInitialize(NULL);

    Application game;
    if ( game.Initialize( hInstance, "DirectX 11 Game", "WindowClass", 1280, 720 ) )
    {
        while ( game.ProcessMessages() == true )
        {
            game.Update();
            game.Render();
        }
    }

    return 0;
}