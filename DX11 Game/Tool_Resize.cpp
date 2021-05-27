#include "stdafx.h"
#include "Tool_Resize.h"

Tool_Resize::Tool_Resize()
{
	ChangeCurrent(0);
}

Tool_Resize::~Tool_Resize()
{
}

void Tool_Resize::ChangeCurrent(int number)
{
	if (number >= 3) {
		number = 0;
	}
	else if (number < 0)
	{
		number = 2;
	}
	CurrentSize = static_cast<BoxSize>(number);

}

void Tool_Resize::ChangeCurrent(std::string name)
{
}

void Tool_Resize::addoneTOCurrent()
{
	current++;
	if (current >= 3) {
		current = 0;
	}
	else if (current < 0)
	{
		current = 2;
	}
	CurrentSize = static_cast<BoxSize>(current);
}

void Tool_Resize::minusoneTOCurrent()
{
	current--;
	if (current >= 3) {
		current = 0;
	}
	else if (current < 0)
	{
		current = 2;
	}
	CurrentSize = static_cast<BoxSize>(current);
}

ToolData Tool_Resize::GetToolData()
{
	ToolData Data;
	Data.boxSize = CurrentSize;
	return Data;
}
