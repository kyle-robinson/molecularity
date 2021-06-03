#pragma once

#include "Event.h"
class Listener
{
public:

	Listener() {}

	virtual ~Listener() {}

	virtual void HandleEvent(Event* event) = 0;

};

