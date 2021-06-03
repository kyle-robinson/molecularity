#include "stdafx.h"
#include "Tool_Resize.h"

Tool_Resize::Tool_Resize()
{
	EnergyCost = 25;
	MaxNumber = 2;
	MinNumber = 0;
	ChangeCurrent( 1 );
}

Tool_Resize::~Tool_Resize()
{ }

void Tool_Resize::ChangeCurrent( int number )
{
	if ( number > MaxNumber )
		number = MinNumber;
	else if ( number < MinNumber )
		number = MaxNumber;
	
	CurrentSize = static_cast<BoxSize>( number );
}

void Tool_Resize::ChangeCurrent( std::string name )
{ }

void Tool_Resize::AddOneToCurrent()
{
	Current++;

	if ( Current > MaxNumber )
		Current = MinNumber;
	else if ( Current < MinNumber )
		Current = MaxNumber;

	CurrentSize = static_cast<BoxSize>( Current );
}

void Tool_Resize::MinusOneTOCurrent()
{
	Current--;

	if ( Current > MaxNumber )
		Current = MinNumber;
	else if ( Current < MinNumber )
		Current = MaxNumber;

	CurrentSize = static_cast<BoxSize>( Current );
}

ToolData Tool_Resize::GetToolData()
{
	ToolData Data;
	Data.boxSize = CurrentSize;
	return Data;
}