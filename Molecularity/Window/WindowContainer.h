#pragma once
#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

/// <summary>
/// Contains a WindowProc which is used to defer window messages to Keyboard.h/Mouse.h for handling.
/// Initialzes and defers raw mouse input to Mouse.h
/// </summary>
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

	DirectX::XMFLOAT2 windowsize;
};

#endif