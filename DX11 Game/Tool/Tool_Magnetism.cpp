#include "stdafx.h"
#include "Tool_Magnetism.h"

Tool_Magnetism::Tool_Magnetism()
{
	
	EnergyCost = 25;
	MaxNumber = 1;
	MinNumber = 0;
	ChangeCurrent(0);
	
}

Tool_Magnetism::~Tool_Magnetism()
{
	
}

void Tool_Magnetism::ChangeCurrent(int number)
{
	if (number > MaxNumber) {
		number = MinNumber;
	}
	else if (number < MinNumber)
	{
		number = MaxNumber;
	}
	CurrentMode = static_cast<MagnetismMode>(number);
}

void Tool_Magnetism::ChangeCurrent(std::string name)
{
}

void Tool_Magnetism::AddOneToCurrent()
{
	Current++;
	if (Current > MaxNumber) {
		Current = MinNumber;
	}
	else if (Current < 0)
	{
		Current = MaxNumber;
	}
	CurrentMode = static_cast<MagnetismMode>(Current);
}

void Tool_Magnetism::MinusOneTOCurrent()
{
	Current--;
	if (Current > MaxNumber) {
		Current = MinNumber;
	}
	else if (Current < 0)
	{
		Current = MaxNumber;
	}
	CurrentMode = static_cast<MagnetismMode>(Current);
}

ToolData Tool_Magnetism::GetToolData()
{

	ToolData Data;
	Data.MagMode = CurrentMode;
	return Data;
}
