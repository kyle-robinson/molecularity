#pragma once
#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(
		WindowContainer* windowContainer,
		HINSTANCE hInstance,
		const std::string& windowTitle,
		const std::string& windowClass,
		int width,
		int height
	);
	bool ProcessMessages() noexcept;
	HWND GetHWND() const noexcept;
	~RenderWindow() noexcept;
private:
	void RegisterWindowClass() noexcept;
	HWND hWnd = NULL;
	HINSTANCE hInstance = NULL;
	std::string windowTitle = "";
	std::wstring windowTitle_Wide = L"";
	std::string windowClass = "";
	std::wstring windowClass_Wide = L"";
	int width, height;
	HCURSOR hCursorNormal = NULL;
};

#endif