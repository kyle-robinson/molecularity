#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "LevelStateMachine.h"
#include "UI_Manager.h"
#include "HUD_UI.h"
#include "Pause.h"
#include "Tutorial_UI.h"
#include "EndLevelScreen_UI.h"

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

	// User Interface
	std::shared_ptr<HUD_UI> HUD;
	std::shared_ptr<Pause> PauseUI;
	std::shared_ptr<Tutorial_UI> TutorialUI;
	std::shared_ptr<EndLevelScreen_UI> EndLevelUI;

	// Scene Objects
	std::unordered_map<std::string, RenderableGameObject> renderables;
};

#endif