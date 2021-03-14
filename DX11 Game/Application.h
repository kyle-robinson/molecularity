#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Input.h"
#include "Timer.h"
#include "Sound.h"
#include "CameraController.h"
#include"Graphics/UI.h"
/// <summary>
/// Calls functions which initialize the window and main graphics components.
/// Responsible for calling main render/update functions in Graphics.h
/// </summary>
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
	Sound sound;
	Graphics gfx;
	CameraController cameras;
	 UI UiControll;
};

#endif