#pragma once
#ifndef TOOL_FUNCTION_H
#define TOOL_FUNCTION_H

#include "Tool_Structs.h"

/// <summary>
/// Manages all the different tool options available to the player.
/// </summary>
class Tool_Function
{
public:
	Tool_Function() {}
	~Tool_Function() {}

	void virtual ChangeCurrent( int number ) = 0;
	void virtual ChangeCurrent( std::string name ) = 0;
	void virtual AddOneToCurrent() = 0;
	void virtual MinusOneTOCurrent() = 0;

	ToolData virtual GetToolData() { return ToolData(); };
	float GetEnergyCost() { return EnergyCost; }
	void SetEnergyCost( float cost ) { EnergyCost = cost; }
protected:
	float EnergyCost = 0.0f;
	int Current = 0;
	int MaxNumber;
	int MinNumber;
};

#endif