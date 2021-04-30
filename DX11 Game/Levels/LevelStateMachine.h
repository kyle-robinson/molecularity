#pragma once
#ifndef LEVELSTATEMACHINE_H
#define LEVELSTATEMACHINE_H

#include "Level.h"
#include <unordered_map>

class LevelStateMachine
{
public:
	LevelStateMachine();
	void Render();
	void Update( const float dt );

	uint32_t Add( std::shared_ptr<Level> level );
	void Remove( uint32_t id );
	void SwitchTo( uint32_t id );
private:
	uint32_t insertedLevelID;
	std::shared_ptr<Level> currentLevel;
	std::unordered_map<uint32_t, std::shared_ptr<Level>> levels;
};

#endif