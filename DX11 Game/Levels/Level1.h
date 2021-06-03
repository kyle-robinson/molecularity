#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "LevelStateMachine.h"

//ui
#include<Graphics/UI_Manager.h>
#include<UI/HUD_UI.h>
#include<UI/Pause.h>
#include<UI/Tutorial_UI.h>
#include<UI/EndLevelScreen_UI.h>

/// <summary>
/// The first level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level1 : public LevelContainer
{
public:
	Level1( LevelStateMachine& stateMachine );
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update( const float dt ) override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;

	// Scene Objects
	std::unordered_map<std::string, RenderableGameObject> renderables;

	//UI
	shared_ptr<HUD_UI> HUD;
	shared_ptr<Pause> PauseUI;
	shared_ptr<Tutorial_UI> TutorialUI;
	shared_ptr<EndLevelScreen_UI> EndLevelUI;

};

#endif