#pragma once
#ifndef LEVEL2_H
#define LEVEL2_H

#include "LevelStateMachine.h"

class Level2 : public Level
{
public:
	Level2( LevelStateMachine& stateMachine );
	bool OnCreate() override;
	void Render() override;
	void RenderFrame() override;
	void Update( const float dt ) override;
private:
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;
	LevelStateMachine& levelStateMachine;
};

#endif