#pragma once
#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "Listener.h"
#include "Event.h"
#include <list>
#include <map>

/// <summary>
/// To add to the event system with a listener
///		EventSystem::Instance()->AddClient(EVENTID::Event1, &listener);
/// To push an event into the system
///		EventSystem::Instance()->AddEvent(EVENTID::Event1, data); data is not needed but possible
/// In the update you need to execute the event buffer with
///		EventSystem::Instance()->ProcessEvents();
/// 
/// List of events stored in Event.h
/// To create new listener type new class inherrited from listener needed
/// </summary>
class EventSystem
{
public:
	// Registers client to specific event
	void AddClient( EVENTID eventid, Listener* client );

	// Checks if the client in question is already in the system. Public because of allowing it to be checked from outside the system if wanted. Might never be called in the public domain though
	bool IsRegistered( EVENTID eventid, Listener* client );

	// Unregisters client from a specific event
	void RemoveClient( EVENTID eventid, Listener* client );

	// Unregisters client from all events
	void RemoveAll( Listener* client );

	// Sends the event to anyone registered
	void SendEvents( Event* event );

	// Add event to the buffer
	void AddEvent( EVENTID eventid, void* data = 0 );

	// Process All events
	void ProcessEvents();

	// Clears all events from the buffer
	void ClearBuffer();

	// Clears all clients from the client list
	void ClearClients();

	// Shutdown event system
	void Shutdown();

	// Returns the event system
	static EventSystem* Instance();
private:

	EventSystem() {};
	~EventSystem() { this->Shutdown(); }

	// Contains all clients and their subscribed events
	std::multimap<EVENTID, Listener*> clients;

	// List of events to process
	std::list<Event> eventsList;
};

#endif