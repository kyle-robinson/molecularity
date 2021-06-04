#include "stdafx.h"
#include "EventSystem.h"

/// Might not be the most efficient Event System in the world (lots of If statements, there might be a way to grab all multimap of type X but i cba)
///	Might not even be more efficient because it might have to also itterate like a for loop
///	Does not allow you to change variables of the incoming data from the event. Only act upon data you may have been given unless ive missed something

// Registers client to specific event
void EventSystem::AddClient( EVENTID eventid, Listener* client )
{
	// If the client event pair doenst exist
	if ( !IsRegistered( eventid, client ) )
		clients.insert( std::make_pair( eventid, client ) );
	else // Outputs that there is a duplicate and doesnt add it to the list of clients since it has no value to have 2
		OutputDebugStringA( "Duplicate client eventID combo\n" );
}

bool EventSystem::IsRegistered( EVENTID eventid, Listener* client )
{
	// Assume the client isnt registered 
	bool exists = false;

	// Iterates the current list of listeners to check if there is a duplicate
	for ( auto i = clients.begin(); i != clients.end(); i++ )
		if ( i->first == eventid )
			if ( i->second == client )
				exists = true;

	return exists;
}

// Unregisters client from a specific event
void EventSystem::RemoveClient( EVENTID eventid, Listener* client ) {
	for ( auto i = clients.begin(); i != clients.end(); i++ )
	{
		if ( i->first == eventid )
		{
			if ( i->second == client )
			{
				// Delete the client in question. No need to keep going since its a unique and more cant exist
				i = clients.erase( i );
				break;
			}
		}
	}
}

// Unregisters client from all events
void EventSystem::RemoveAll( Listener* client )
{
	for ( auto i = clients.begin(); i != clients.end(); i++ )
		if ( i->second == client )
			i = clients.erase( i ); //Delete this client, keep going
}

// Process specific event. Add to event buffer
void EventSystem::AddEvent( EVENTID eventid, void* data ) {
	Event newEvent( eventid, data );
	//Add to the buffer
	eventsList.push_back( newEvent );
}

// Process event buffer
void EventSystem::ProcessEvents()
{
	while ( !eventsList.empty() )
	{
		SendEvents( &eventsList.front() );
		//Remove the event from the buffer
		eventsList.pop_front();
	}
}

// Sends the event to the listener
void EventSystem::SendEvents( Event* event )
{
	for ( auto i = clients.begin(); i != clients.end(); i++ )
		if ( i->first == event->GetEventID() )
			i->second->HandleEvent( event );
}

// Clears the whole Events buffer
void EventSystem::ClearBuffer()
{
	eventsList.clear();
}

// Clears the whole client list
void EventSystem::ClearClients()
{
	clients.clear();
}

// Shutdown event system
void EventSystem::Shutdown()
{
	ClearBuffer();
	ClearClients();
}

EventSystem* EventSystem::Instance()
{
	static EventSystem instance;
	return &instance;
}