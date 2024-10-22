#include "stdafx.h"
#include "LevelStateMachine.h"

LevelStateMachine::LevelStateMachine() : levels( 0 ), currentLevel( 0 ) { AddToEvent(); }

void LevelStateMachine::Update( const float dt )
{
	if ( currentLevel )
		currentLevel->Update( dt );
}

void LevelStateMachine::Render()
{
	if ( currentLevel )
		currentLevel->Render();
}

uint32_t LevelStateMachine::Add( std::shared_ptr<LevelContainer> level )
{
	auto inserted = levels.insert( std::make_pair( insertedLevelID, level ) );
	insertedLevelID++;
	return insertedLevelID - 1;
}

void LevelStateMachine::Remove( uint32_t id )
{
	auto it = levels.find( id );
	if ( it != levels.end() )
		if ( currentLevel == it->second )
			currentLevel = nullptr;
	levels.erase( it );
}

void LevelStateMachine::SwitchTo( uint32_t id )
{
	auto it = levels.find( id );
	if ( it != levels.end() )
	{
		if (currentLevel)
			currentLevel->CleanUp();
		Sound::Instance()->ClearAudio();
		currentLevel = it->second;
		currentLevel->OnCreate();
		currentLevel->OnSwitch();
	}
}

void LevelStateMachine::AddToEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::GameLevelChangeEvent, this );
}

void LevelStateMachine::HandleEvent( Event* event )
{
	// Switch level
	switch ( event->GetEventID() )
	{
	case EVENTID::GameLevelChangeEvent:
	{
		SwitchTo( *static_cast<int*>( event->GetData() ) );
	}
	break;
	}
}