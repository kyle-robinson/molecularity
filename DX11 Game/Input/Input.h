#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "MousePicking.h"
#include "WindowContainer.h"
#include "LevelStateMachine.h"

#include<UI/UI.h>
/// <summary>
/// Handles user input by update keyboard and mouse events.
/// Contains functions to update game state on successful keyboard/mouse event registers.
/// </summary>
class Input : public WindowContainer, public Listener
{
public:
	void Initialize( RenderWindow& window, LevelStateMachine* stateMachine,
		CameraController* camera );
	void Update( const float dt );

	//eventsystem
	void AddToEvent();
	void HandleEvent(Event* event);
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



	//ui input
	bool isPaused;
	unsigned char UIChar;
	MouseData UiMouseData;

	//key binds 
	std::unordered_map<string, unsigned char> KeyBinds;
	std::unordered_map<string, Mouse::MouseEvent::EventType> MouseBinds;

	bool heldLastFrame[2] = { false };
};

#endif