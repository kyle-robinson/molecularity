#pragma once
#ifndef LISTENER_H
#define LISTENER_H

#include "Event.h"

/// <summary>
/// Classes that inherit from this class can listen for events from elsewhere in the system.
/// </summary>
class Listener
{
public:
	Listener() {}
	virtual ~Listener() {}
	virtual void HandleEvent( Event* event ) = 0;
};

#endif