#pragma once



#include<Tool_Structs.h>
/// <summary>
/// hold the current tool data to then pass to cube when picked
/// </summary>
#include<Tool_Function.h>

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

	//eventsystem
	void AddToEvent();
	void HandleEvent(Event* event);


private:
	

private:
	//currently selected
	std::shared_ptr<Tool_Function> _CurrentTool;
	ToolType _ToolType;



};

