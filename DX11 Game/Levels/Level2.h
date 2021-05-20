#pragma once
#ifndef LEVEL2_H
#define LEVEL2_H

#include "Sprite.h"
#include "LevelStateMachine.h"

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
	Sprite crosshair;
	RenderableGameObject hubRoom;
};

#endif