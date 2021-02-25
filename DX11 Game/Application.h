#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

/// <summary>
/// Calls functions which initialize the window and main graphics components.
/// Manages the keyboard/mouse input for the scene. (Needs refactored at some point.)
/// Responsible for calling main render/update functions in Graphics.h
/// </summary>

#include "Input.h"
#include "Timer.h"
#include "CameraController.h"

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
	Input input;
	Timer timer;
	Graphics gfx;
	CameraController cameras;
};

#endif