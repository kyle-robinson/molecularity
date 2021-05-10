#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "Sprite.h"
#include "LevelStateMachine.h"

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
	void ProcessInput() override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;

	// Scene Objects
	Sprite crosshair;
	RenderableGameObject room;
	RenderableGameObject podium;
	RenderableGameObject pressurePlate;
};

#endif