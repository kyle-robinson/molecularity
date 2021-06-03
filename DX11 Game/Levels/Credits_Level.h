#pragma once
#ifndef CREDITS_LEVEL_H
#define CREDITS_LEVEL_H

#include "LevelContainer.h"
#include "LevelStateMachine.h"
#include "UI_Manager.h"
#include "Credits_UI.h"

/// <summary>
/// The end credits of the game. Displays the developers and their roles throughout the development process.
/// </summary>
class Credits_Level : public LevelContainer
{
public:
	Credits_Level( LevelStateMachine& stateMachine );
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update(const float dt) override;
private:
	void RenderFrame() override;

	std::shared_ptr<Credits_UI> credits;
	LevelStateMachine& levelStateMachine;
};

#endif