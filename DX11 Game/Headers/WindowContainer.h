#pragma once
#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include "Graphics.h"
#include "RenderWindow.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
protected:
	RenderWindow renderWindow;
	Graphics gfx;
};

#endif