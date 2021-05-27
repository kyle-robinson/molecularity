#pragma once
#ifndef MAINMENU_LEVEL_H
#define MAINMENU_LEVEL_H

#include "LevelStateMachine.h"
//ui
#include<Graphics/UI_Manager.h>
#include<UI/Settings_Menu_UI.h>
#include<UI/Main_Menu_UI.h>

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

	shared_ptr<Main_Menu_UI> Menu;
	shared_ptr<Settings_Menu_UI> settingsUi;
};

#endif