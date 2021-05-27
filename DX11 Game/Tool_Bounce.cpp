#include "stdafx.h"
#include "Tool_Bounce.h"

Tool_Bounce::Tool_Bounce()
{
	EnergyCost = 25;
	MaxNumber = 1;
	MinNumber = 0;
	CurrentType = BoxBounce::Solid;
}

Tool_Bounce::~Tool_Bounce()
{
}

void Tool_Bounce::ChangeCurrent( int number )
{

	if ( number > MaxNumber) {
		number = MinNumber;
	}
	else if ( number < MinNumber)
	{
		number = MaxNumber;
	}
	CurrentType = static_cast< BoxBounce >( number );
}

void Tool_Bounce::ChangeCurrent( std::string name )
{
}

void Tool_Bounce::AddOneToCurrent()
{
	Current++;
	if ( Current > MaxNumber) {
		Current = MinNumber;
	}
	else if ( Current < MinNumber)
	{
		Current = 1;
	}
	CurrentType = static_cast< BoxBounce >( Current );
}

void Tool_Bounce::MinusOneTOCurrent()
{
	Current--;

	if ( Current > MaxNumber) {
		Current = MinNumber;
	}
	else if ( Current < MinNumber)
	{
		Current = MaxNumber;
	}

	CurrentType = static_cast< BoxBounce >( Current );
}

ToolData Tool_Bounce::GetToolData()
{
	ToolData Data;
	Data.boxBounce = CurrentType;
	return Data;
}