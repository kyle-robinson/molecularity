#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Input.h"
#include "Timer.h"
#include "ImGuiManager.h"

#include "Level1.h"
#include "Level2.h"
#include"MainMenu_Level.h"

#include"Graphics\\UI_Manager.h"

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
	UI_Manager _UI_Manager;
	ImGuiManager imgui;
	CameraController cameras;
	
	// Levels
	uint32_t level1_ID;
	uint32_t level2_ID;
	uint32_t MainMenu_ID;
	std::shared_ptr<Level1> level1;
	std::shared_ptr<Level2> level2;
	std::shared_ptr<MainMenu_Level> MainMenu;
	LevelStateMachine stateMachine;

	std::vector<JSON::SettingData> _SettingsData;
};

#endif