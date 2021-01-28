#pragma once
#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include "Graphics.h"
#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
protected:
	RenderWindow renderWindow;
	Keyboard keyboard;
	Graphics gfx;
	Mouse mouse;
};

#endif