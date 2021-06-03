#pragma once
#ifndef LEVEL2_H
#define LEVEL2_H

#include "LevelStateMachine.h"
#include "UI_Manager.h"
#include "HUD_UI.h"
#include "Pause.h"
#include "EndLevelScreen_UI.h"

/// <summary>
/// The second level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level2 : public LevelContainer
{
public:
	Level2( LevelStateMachine& stateMachine );
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update( const float dt ) override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;

	// Scene Objects
	std::unordered_map<std::string, RenderableGameObject> renderables;

	// User Interface
	std::shared_ptr<HUD_UI> HUD;
	std::shared_ptr<Pause> PauseUI;
	std::shared_ptr<EndLevelScreen_UI> EndLevelUI;
};

#endif