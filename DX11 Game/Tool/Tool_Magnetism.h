#pragma once
#ifndef TOOL_MAGNETISM_H
#define TOOL_MAGNETISM_H

#include "Tool_Function.h"

/// <summary>
/// This tool makes the cubes magnetic, pulling them towards the player.
/// </summary>
class Tool_Magnetism : public Tool_Function
{
public:
	Tool_Magnetism();
	~Tool_Magnetism();

	void ChangeCurrent( int number )override;
	void ChangeCurrent( std::string name )override;
	void AddOneToCurrent() override;
	void MinusOneTOCurrent() override;

	MagnetismMode GetMagMode() { return CurrentMode; }
	ToolData GetToolData() override;
private:
	MagnetismMode CurrentMode;
};

#endif