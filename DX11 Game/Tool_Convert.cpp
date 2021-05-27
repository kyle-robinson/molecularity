#include "stdafx.h"
#include "Tool_Convert.h"

Tool_Convert::Tool_Convert()
{
	CurrentType = BoxType::Mesh;
}

Tool_Convert::~Tool_Convert()
{
}

void Tool_Convert::ChangeCurrent(int number)
{
	
	if (number > 5) {
		number = 0;
	}
	else if(number <0)
	{
		number = 4;
	}
	CurrentType = static_cast<BoxType>(number);
}

void Tool_Convert::ChangeCurrent(std::string name)
{
}

void Tool_Convert::addoneTOCurrent()
{
	current++;
	if (current > 4) {
		current = 0;
	}
	else if (current < 0)
	{
		current = 4;
	}
	CurrentType = static_cast<BoxType>(current);
}

void Tool_Convert::minusoneTOCurrent()
{
	current--;

	if (current > 5) {
		current = 0;
	}
	else if (current < 0)
	{
		current = 4;
	}


	CurrentType = static_cast<BoxType>(current);
}

ToolData Tool_Convert::GetToolData()
{
	ToolData Data;
	Data.boxtype = CurrentType;
	return Data;
}
