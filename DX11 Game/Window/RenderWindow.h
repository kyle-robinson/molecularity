#pragma once
#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include "ErrorLogger.h"
#include "EventSystem\\EventSystem.h"
class WindowContainer;
;
/// <summary>
/// Create/intialize the main application window.
/// Handle the processing of messages.
/// </summary>
class RenderWindow:public Listener
{
public:
	~RenderWindow() noexcept;
	bool Initialize(
		WindowContainer* windowContainer,
		HINSTANCE hInstance,
		const std::string& windowTitle,
		const std::string& windowClass,
		int width,
		int height
	);
	bool ProcessMessages() noexcept;

	HWND GetHWND() const noexcept { return hWnd; }
	UINT GetWidth() const noexcept { return static_cast<UINT>( width ); };
	UINT GetHeight() const noexcept { return static_cast<UINT>( height ); };

	void SetWidthHight(UINT Width, UINT Hight) { width = Width; height = Hight; }

	void HandleEvent(Event* event);

	HCURSOR hCursorNormal = NULL;
	HCURSOR hCursorNightNormal = NULL;
	HCURSOR hCursorNightSelect = NULL;
private:
	void RegisterWindowClass() noexcept;

	HWND hWnd = NULL;
	HINSTANCE hInstance = NULL;
	std::string windowTitle = "";
	std::wstring windowTitle_Wide = L"";
	std::string windowClass = "";
	std::wstring windowClass_Wide = L"";
	int width = 0, height = 0;
};

#endif