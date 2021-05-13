#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Sound.h"
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
		CameraController* camera, Sound* sound, std::vector<uint32_t> level_IDs );
	void Update( const float dt );

	//eventsystem
	void AddToEvent();
	void HandleEvent(Event* event);


private:
	void UpdateKeyboard( const float dt );
	void UpdateMouse( const float dt );

	Sound* soundSystem;
	LevelContainer* level;
	MousePicking mousePick;
	CameraController* cameras;
	LevelStateMachine* levelSystem;
	std::vector<uint32_t> level_IDs;
	
	//key bindes 
	map<string, unsigned char> KeyBindes;
	map<string, Mouse::MouseEvent::EventType> MouseBindes;
  


	ToolType currentTool;
	//ui input
	unsigned char UIChar;
	MouseData UiMouseData;
};

#endif