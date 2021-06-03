#include "stdafx.h"
#include "Tool_Conductivity.h"

Tool_Conductivity::Tool_Conductivity()
{
	EnergyCost = 25;
	MaxNumber = 1;
	MinNumber = 0;
	ChangeCurrent(0);

}

Tool_Conductivity::~Tool_Conductivity()
{
}

void Tool_Conductivity::ChangeCurrent(int number)
{

	if (number > MaxNumber) {
		number = MinNumber;
	}
	else if (number < MinNumber)
	{
		number = MaxNumber;
	}
	CurrentConductive = static_cast<BoxConductive>(number);
}

void Tool_Conductivity::ChangeCurrent(std::string name)
{
}

void Tool_Conductivity::AddOneToCurrent()
{
	Current++;
	if (Current > MaxNumber) {
		Current = MinNumber;
	}
	else if (Current < MinNumber)
	{
		Current = MaxNumber;
	}
	CurrentConductive = static_cast<BoxConductive>(Current);
}

void Tool_Conductivity::MinusOneTOCurrent()
{
	Current++;
	if (Current > MaxNumber) {
		Current = MinNumber;
	}
	else if (Current < MinNumber)
	{
		Current = MaxNumber;
	}
	CurrentConductive = static_cast<BoxConductive>(Current);
}

ToolData Tool_Conductivity::GetToolData()
{
	ToolData Data;
	Data.boxConductive = CurrentConductive;
	return Data;
}
