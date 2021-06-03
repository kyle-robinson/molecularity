#pragma once
#ifndef TOOL_BOUNCE_H
#define TOOL_BOUNCE_H

#include "Tool_Function.h"
#include "Tool_Structs.h"

/// <summary>
/// This tool mode allows the player to make the cube either bouncy or solid.
/// </summary>
class Tool_Bounce : public Tool_Function
{
public:
	Tool_Bounce();
	~Tool_Bounce();

	void ChangeCurrent( int number )override;
	void ChangeCurrent( std::string name )override;
	void AddOneToCurrent() override;
	void MinusOneTOCurrent() override;

	BoxBounce GetType() { return CurrentType; }
	ToolData  GetToolData() override;
private:
	BoxBounce CurrentType;
};

#endif