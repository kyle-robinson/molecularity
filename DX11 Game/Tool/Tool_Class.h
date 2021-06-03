#pragma once
#ifndef TOOL_CLASS_H
#define TOOL_CLASS_H

#include "Timer.h"
#include "Tool_Structs.h"
#include "Tool_Function.h"
class CubeProperties;

/// <summary>
/// Hold the current tool data to then pass to cube when picked.
/// </summary>
class Tool_Class :public Listener
{
public:
	Tool_Class();
	~Tool_Class();

	void SetCurrentTool( ToolType CurrentTool );

	Tool_Function* GetCurrentTool();
	ToolType GetTooltype() { return _ToolType; }
	ToolData GetCurrentOption();

	void Update();

	// Energy System
	float GetEnergy() { return _Energy; }
	void SetEnergy( float energy ) { _Energy = energy; }

	float GetEnergyMax() { return _EnergyMax; }
	void SetEnergyMax( float energyMax ) { _EnergyMax = energyMax; }

	// Event System
	void HandleEvent( Event* event );
private:
	void ChangeCube( CubeProperties* Cube );
	void AddToEvent();
	void RemoveFromEvent();
private:
	// Currently selected
	std::shared_ptr<Tool_Function> _CurrentTool;
	ToolType _ToolType;
	Timer timer;
	float _Energy = 100;
	float _EnergyMax = 100;
};

#endif