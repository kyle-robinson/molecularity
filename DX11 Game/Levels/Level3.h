#pragma once
#ifndef LEVEL3_H
#define LEVEL3_H

#include "LevelStateMachine.h"
#include "UI_Manager.h"
#include "HUD_UI.h"
#include "Pause.h"
#include "EndLevelScreen_UI.h"

/// <summary>
/// The third level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level3 : public LevelContainer
{
public:
	Level3( LevelStateMachine& stateMachine );
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update( const float dt ) override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;

	// Scene Objects
	std::unordered_map<std::string, RenderableGameObject> renderables;
	std::vector<std::pair<XMFLOAT3, bool>> brokenCircuitPoints;
	bool renderDoor = true;

	// User Interface
	shared_ptr<HUD_UI> HUD;
	shared_ptr<Pause> PauseUI;
	shared_ptr<EndLevelScreen_UI> EndLevelUI;
};

#endif