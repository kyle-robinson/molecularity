#pragma once
#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include "GraphicsContainer.h"
#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void EnableCursor()noexcept;
	void DisableCursor()noexcept;
protected:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowCursor() noexcept;
	void HideCursor() noexcept;
	void EnableImGuiMouse() noexcept;
	void DisableImGuiMouse() noexcept;

	bool cursorEnabled = false;
	RenderWindow renderWindow;
	GraphicsContainer gfx;
	Keyboard keyboard;
	Mouse mouse;
};

#endif