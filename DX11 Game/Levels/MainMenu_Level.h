#pragma once
#ifndef MAINMENU_LEVEL_H
#define MAINMENU_LEVEL_H

#include "LevelStateMachine.h"
#include "UI_Manager.h"
#include "Main_Menu_UI.h"
#include "Settings_Menu_UI.h"

class MainMenu_Level : public LevelContainer
{
public:
	MainMenu_Level(LevelStateMachine& stateMachine);
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update(const float dt) override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;
	std::shared_ptr<Main_Menu_UI> Menu;
	std::shared_ptr<Settings_Menu_UI> settingsUi;
};

#endif