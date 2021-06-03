#pragma once
#ifndef LEVEL2_H
#define LEVEL2_H

#include "LevelStateMachine.h"

//ui
#include<Graphics/UI_Manager.h>
#include<UI/HUD_UI.h>
#include<UI/Pause.h>
#include<UI/EndLevelScreen_UI.h>

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

	// UI
	shared_ptr<HUD_UI> HUD;
	shared_ptr<Pause> PauseUI;
	shared_ptr<EndLevelScreen_UI> EndLevelUI;
};

#endif