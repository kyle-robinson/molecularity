#pragma once
#ifndef TOOL_RESIZE_H
#define TOOL_RESIZE_H

#include "Tool_Function.h"
#include "Tool_Structs.h"

/// <summary>
/// This tool mode handles the resizing of the cubes.
/// </summary>
class Tool_Resize : public Tool_Function
{
public:
	Tool_Resize();
	~Tool_Resize();

	void ChangeCurrent( int number )override;
	void ChangeCurrent( std::string name )override;
	void  AddOneToCurrent() override;
	void  MinusOneTOCurrent() override;

	BoxSize GetSize() { return CurrentSize; }
	ToolData GetToolData() override;
private:
	BoxSize CurrentSize;
};

#endif