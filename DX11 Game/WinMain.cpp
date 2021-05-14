#include "Application.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow );
  
    HRESULT hr = CoInitializeEx( NULL, COINITBASE_MULTITHREADED );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to call CoInitialize!" );
        return -1;
    }

    Application game;
    if ( game.Initialize( hInstance, "DirectX 11 Group Project - Molecularity", "WindowClass", 2560, 1440) )
    {
        while ( game.ProcessMessages() == true )
        {
            game.Update();
            game.Render();
        }
    }

    return 0;
}