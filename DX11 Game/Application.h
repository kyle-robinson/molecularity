#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

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