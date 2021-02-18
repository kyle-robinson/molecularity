#pragma once
#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

/// <summary>
/// Contains a WindowProc which is used to defer window messages to Keyboard.h/Mouse.h for handling.
/// Initialzes and defers raw mouse input to Mouse.h
/// </summary>

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
	Keyboard keyboard;
	Mouse mouse;
};

#endif