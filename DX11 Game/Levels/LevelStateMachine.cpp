#include "stdafx.h"
#include "LevelStateMachine.h"

LevelStateMachine::LevelStateMachine() : levels( 0 ), currentLevel( 0 ) { }

void LevelStateMachine::Update( const float dt )
{
	if ( currentLevel )
	{
		currentLevel->Update( dt );
	}
}

void LevelStateMachine::Render()
{
	if ( currentLevel )
	{
		currentLevel->Render();
	}
}

uint32_t LevelStateMachine::Add( std::shared_ptr<LevelContainer> level )
{
	auto inserted = levels.insert( std::make_pair( insertedLevelID, level ) );
	insertedLevelID++;
	inserted.first->second->OnCreate();
	return insertedLevelID - 1;
}

void LevelStateMachine::Remove( uint32_t id )
{
	auto it = levels.find( id );
	if ( it != levels.end() )
	{
		if ( currentLevel == it->second )
		{
			currentLevel = nullptr;
		}
	}
	levels.erase( it );
}

void LevelStateMachine::SwitchTo( uint32_t id )
{
	auto it = levels.find( id );
	if ( it != levels.end() )
	{
		currentLevel = it->second;
	}
}