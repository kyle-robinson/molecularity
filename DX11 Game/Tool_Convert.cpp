#include "stdafx.h"
#include "Tool_Convert.h"

Tool_Convert::Tool_Convert()
{
	EnergyCost = 25;
	CurrentType = BoxType::Mesh;
	MaxNumber = 4;
	MinNumber = 0;
	ChangeCurrent(0);
}

Tool_Convert::~Tool_Convert()
{
}

void Tool_Convert::ChangeCurrent(int number)
{
	
	if (number > MaxNumber) {
		number = MinNumber;
	}
	else if(number < MinNumber)
	{
		number = MaxNumber;
	}
	CurrentType = static_cast<BoxType>(number);
}

void Tool_Convert::ChangeCurrent(std::string name)
{
}


void Tool_Convert::AddOneToCurrent()
{
	Current++;
	if (Current > MaxNumber) {
		Current = MinNumber;
	}
	else if (Current < 0)
	{
		Current = MaxNumber;
	}
	CurrentType = static_cast<BoxType>(Current);
}

void Tool_Convert::MinusOneTOCurrent()
{
	Current--;

	if (Current > MaxNumber) {
		Current = MinNumber;
	}
	else if (Current < MinNumber)
	{
		Current = MaxNumber;
	}


	CurrentType = static_cast<BoxType>(Current);
}

ToolData Tool_Convert::GetToolData()
{
	ToolData Data;
	Data.boxtype = CurrentType;
	return Data;
}
