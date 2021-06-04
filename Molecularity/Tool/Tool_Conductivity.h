#pragma once
#ifndef TOOL_CONDUCTIVITY_H
#define TOOL_CONDUCTIVITY_H

#include "Tool_Function.h"

/// <summary>
/// This tool mode makes the cubes conductive, allowing them to complete circuits.
/// </summary>
class Tool_Conductivity : public Tool_Function
{
public:
	Tool_Conductivity();
	~Tool_Conductivity();

	void ChangeCurrent( int number )override;
	void ChangeCurrent( std::string name )override;
	void AddOneToCurrent() override;
	void MinusOneTOCurrent() override;

	BoxConductive GetType() { return CurrentConductive; }
	ToolData  GetToolData() override;
private:
	BoxConductive CurrentConductive;
};

#endif