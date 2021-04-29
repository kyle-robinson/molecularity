#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Level.h"
#include "MousePicking.h"
#include "WindowContainer.h"
class Sound;

/// <summary>
/// Handles user input by update keyboard and mouse events.
/// Contains functions to update game state on successful keyboard/mouse event registers.
/// </summary>
class Input : public WindowContainer
{
public:
	void Initialize( Level* gfx, RenderWindow& window,CameraController* camera, int width, int height );
	void Update( const float dt, Sound sound );
private:
	void UpdateKeyboard( const float dt );
	void UpdateMouse( const float dt );

	Level* level;
	MousePicking mousePick;
	CameraController* cameras;
};

#endif