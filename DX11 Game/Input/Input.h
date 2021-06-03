#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "UI.h"
#include "MousePicking.h"
#include "WindowContainer.h"
#include "LevelStateMachine.h"

/// <summary>
/// Handles user input by update keyboard and mouse events.
/// Contains functions to update game state on successful keyboard/mouse event registers.
/// </summary>
class Input : public WindowContainer, public Listener
{
public:
	void Initialize( RenderWindow& window, LevelStateMachine* stateMachine,
		CameraController* camera, std::vector<uint32_t> level_IDs );
	void Update( const float dt );

	// Event System
	void AddToEvent();
	void HandleEvent( Event* event );
private:
	void UpdateKeyboard( const float dt );
	void UpdateMouse( const float dt );

	ToolType currentTool;
	LevelContainer* level;
	MousePicking mousePick;
	CameraController* cameras;
	LevelStateMachine* levelSystem;
	std::vector<uint32_t> level_IDs;

	float delay = 0.0f;
	bool canHover = true;

	// UI Input
	bool isPaused;
	unsigned char UIChar;
	MouseData UiMouseData;

	// Key Binds 
	std::unordered_map<std::string, unsigned char> KeyBinds;
	std::unordered_map<std::string, Mouse::MouseEvent::EventType> MouseBinds;

	bool heldLastFrame[2] = { false };
};

#endif