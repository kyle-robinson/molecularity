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

void Tool_Bounce::AddOneToCurrent()
{
	Current++;
	if ( Current > 1 ) {
		Current = 0;
	}
	else if ( Current < 0 )
	{
		Current = 1;
	}
	CurrentType = static_cast< BoxBounce >( Current );
}

void Tool_Bounce::MinusOneTOCurrent()
{
	Current--;

	if ( Current > 2 ) {
		Current = 0;
	}
	else if ( Current < 0 )
	{
		Current = 1;
	}

	CurrentType = static_cast< BoxBounce >( Current );
}

ToolData Tool_Bounce::GetToolData()
{
	ToolData Data;
	Data.boxBounce = CurrentType;
	return Data;
}