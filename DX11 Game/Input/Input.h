#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Graphics.h"
#include "MousePicking.h"
#include "WindowContainer.h"

/// <summary>
/// Handles all user inputs to the system
/// </summary>
class Input : public WindowContainer
{
public:
	void Initialize(Graphics* graphicDevice,RenderWindow* renderDevice, int width, int height);
	void Update(const float dt);

private:
	void UpdateKeyboard(const float dt);
	void UpdateMouse(const float dt);


private:
	Graphics* graphics;
	MousePicking mousePick;
};

#endif