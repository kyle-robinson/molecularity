#include "stdafx.h"
#include "Application.h"

bool Application::Initialise()
{
	return true;
}

bool Application::HandleKeyboard( MSG msg )
{
	switch ( msg.wParam )
	{
	// window management
	case VK_ESCAPE:
		PostQuitMessage( 0 );
		return true;
		break;
	}
	return false;
}

void Application::Update()
{

}

void Application::Draw()
{

}