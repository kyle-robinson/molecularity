#pragma once

#include<Tool_Structs.h>
#include<Timer.h>
#include<Tool_Function.h>

/// <summary>
/// hold the current tool data to then pass to cube when picked
/// </summary>
class CubeProperties;
class Tool_Class:public Listener
{
public:
	Tool_Class();
	~Tool_Class();
	
	void SetCurrentTool(Tool_Function* CurrentTool);
	void SetCurrentTool(ToolType CurrentTool);
	Tool_Function* GetCurrentTool();
	ToolType GetTooltype() { return _ToolType; }
	ToolData GetCurrentOption();
	void Update();
	void Draw();

	//energy system
	float GetEnergy() { return _Energy; }
	void setEnergy(float energy) { _Energy = energy; }

	float GetEnergyMax() { return _EnergyMax; }
	void setEnergyMax(float energyMax) { _EnergyMax = energyMax; }

	//eventsystem
	
	void HandleEvent(Event* event);


private:
	void ChangeCube(CubeProperties* Cube);
	void AddToEvent();
	void RemoveFromEvent();
private:
	//currently selected
	std::shared_ptr<Tool_Function> _CurrentTool;
	ToolType _ToolType;
	Timer timer;
	float _Energy=100;
	float _EnergyMax=100;
};

