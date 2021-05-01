#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "MousePicking.h"
#include "WindowContainer.h"
#include "LevelStateMachine.h"
class Sound;

/// <summary>
/// Handles user input by update keyboard and mouse events.
/// Contains functions to update game state on successful keyboard/mouse event registers.
/// </summary>
class Input : public WindowContainer
{
public:
	void Initialize( RenderWindow& window, LevelStateMachine* stateMachine,
		CameraController* camera, std::vector<uint32_t> level_IDs );
	void Update( const float dt, Sound sound );
private:
	void UpdateKeyboard( const float dt );
	void UpdateMouse( const float dt );

	LevelContainer* level;
	MousePicking mousePick;
	CameraController* cameras;
	LevelStateMachine* levelSystem;
	std::vector<uint32_t> level_IDs;
};

#endif