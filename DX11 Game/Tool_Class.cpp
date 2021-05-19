#include "stdafx.h"
#include "Tool_Class.h"
#include<Tool_Convert.h>
#include<Tool_Resize.h>
#include<Objects/CubeProperties.h>

Tool_Class::Tool_Class()
{
	
	SetCurrentTool(ToolType::Convert);
	AddToEvent();
	EventSystem::Instance()->AddEvent(EVENTID::ToolModeEvent, this);
}

Tool_Class::~Tool_Class()
{
	
}

void Tool_Class::SetCurrentTool(Tool_Function* CurrentTool)
{
	//_CurrentTool = std::make_shared <Tool_Function>(CurrentTool);
}

void Tool_Class::SetCurrentTool(ToolType CurrentTool)
{

	switch (CurrentTool)
	{
	case ToolType::Convert:
		
		_CurrentTool = std::make_shared<Tool_Convert>();
		_ToolType = CurrentTool;
		break;
	case ToolType::Resize:
		_CurrentTool = std::make_shared<Tool_Resize>();
		_ToolType = CurrentTool;
		break;
	default:
		break;
	}

}

Tool_Function* Tool_Class::GetCurrentTool()
{
	return _CurrentTool.get();
}

ToolData Tool_Class::GetCurrentOption()
{
	
	return _CurrentTool->GetToolData();
}

void Tool_Class::Update()
{
	// manage multi-tool options
	



	//energy regen
	if (_Energy < _EnergyMax && timer.GetMilliSecondsElapsed()>=1000) {
		_Energy += 1;
	}
	else if (_Energy == _EnergyMax) {
		timer.Stop();
		timer.Restart();
	}

}

void Tool_Class::AddToEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::ChangeToolEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeToolOptionEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeToolOptionDownEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeToolOptionUpEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeCubeEvent, this);
}



void Tool_Class::HandleEvent(Event* event)
{
	//chnage tool
	switch (event->GetEventID())
	{
	case EVENTID::ChangeToolOptionEvent:
	{
		_CurrentTool->ChangeCurrent(*static_cast<int*>(event->GetData()));
	}
	break;
	case EVENTID::ChangeToolOptionUpEvent:
	{
	
		_CurrentTool->addoneTOCurrent();
	}
	break;
	case EVENTID::ChangeToolOptionDownEvent:
	{
		
		_CurrentTool->minusoneTOCurrent();
	}
	break;
	case EVENTID::ChangeToolEvent:
	{
		SetCurrentTool(*static_cast<ToolType*>(event->GetData()));
	}
	break;
	case EVENTID::ChangeCubeEvent:
	{
		
		timer.Restart();
		if (_Energy >= 0) {
			_Energy -= 25;
			timer.Start();
		}
		 CubeProperties* cube=static_cast<CubeProperties*>(event->GetData());

		 switch (_ToolType)
		 {
		 case ToolType::Convert:
			 cube->SetBoxType(_CurrentTool->GetToolData().boxtype);
			 break;
		 case ToolType::Resize:
			 cube->SetBoxSize(_CurrentTool->GetToolData().boxSize);
			 break;
		 default:
			 break;
		 }
	

	}
	break;

	}

}


//TODO Get Cubes
//TODO Get Picking