#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Graphics.h"
#include "MousePicking.h"
#include "WindowContainer.h"

/// <summary>
/// Handles user input by update keyboard and mouse events.
/// Contains functions to update game state on successful keyboard/mouse event registers.
/// </summary>
class Input : public WindowContainer
{
public:
	void Initialize( Graphics* gfx, RenderWindow& window,CameraController* camera, int width, int height );
	void Update( const float dt);

private:
	void UpdateKeyboard( const float dt );
	void UpdateMouse( const float dt );

	MousePicking mousePick;
	Graphics* graphics;
	CameraController* cameras;
	
};

#endif