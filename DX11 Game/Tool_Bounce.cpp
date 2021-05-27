#include "stdafx.h"
#include "Tool_Bounce.h"

Tool_Bounce::Tool_Bounce()
{
	CurrentType = BoxBounce::Solid;
}

Tool_Bounce::~Tool_Bounce()
{
}

void Tool_Bounce::ChangeCurrent( int number )
{

	if ( number > 2 ) {
		number = 0;
	}
	else if ( number < 0 )
	{
		number = 1;
	}
	CurrentType = static_cast< BoxBounce >( number );
}

void Tool_Bounce::ChangeCurrent( std::string name )
{
}

void Tool_Bounce::addoneTOCurrent()
{
	current++;
	if ( current > 1 ) {
		current = 0;
	}
	else if ( current < 0 )
	{
		current = 1;
	}
	CurrentType = static_cast< BoxBounce >( current );
}

void Tool_Bounce::minusoneTOCurrent()
{
	current--;

	if ( current > 2 ) {
		current = 0;
	}
	else if ( current < 0 )
	{
		current = 1;
	}

	CurrentType = static_cast< BoxBounce >( current );
}

ToolData Tool_Bounce::GetToolData()
{
	ToolData Data;
	Data.boxBounce = CurrentType;
	return Data;
}