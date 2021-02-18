#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

/// <summary>
/// Calls functions which initialize the window and main graphics components.
/// Manages the keyboard/mouse input for the scene. (Needs refactored at some point.)
/// Responsible for calling main render/update functions in Graphics.h
/// </summary>

#include "Timer.h"
#include "Graphics.h"
#include "MousePicking.h"
#include "WindowContainer.h"

class Application : public WindowContainer
{
public:
	bool Initialize(
		HINSTANCE hInstance,
		const std::string& windowTitle,
		const std::string& windowClass,
		int width,
		int height
	);
	bool ProcessMessages() noexcept;
	void Update();
	void Render();
private:
	Timer timer;
	Graphics graphics;
	MousePicking mousePick;
};

#endif