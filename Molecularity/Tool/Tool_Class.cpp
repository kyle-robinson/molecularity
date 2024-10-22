#include "stdafx.h"
#include "Tool_Class.h"
#include "Tool_Convert.h"
#include "Tool_Resize.h"
#include "Tool_Magnetism.h"
#include "Tool_Bounce.h"
#include "Tool_Conductivity.h"
#include "CubeProperties.h"
#include "Cube.h"

Tool_Class::Tool_Class()
{
	_ToolType = ToolType::Convert;
	SetCurrentTool( ToolType::Convert );
	AddToEvent();
}

Tool_Class::~Tool_Class()
{
	RemoveFromEvent();
}

void Tool_Class::SetCurrentTool( ToolType CurrentTool )
{
	switch ( CurrentTool )
	{
	case ToolType::Convert:
		_CurrentTool = std::make_shared<Tool_Convert>();
		_ToolType = CurrentTool;
		break;

	case ToolType::Resize:
		_CurrentTool = std::make_shared<Tool_Resize>();
		_ToolType = CurrentTool;
		break;

	case ToolType::Magnetism:
		_CurrentTool = std::make_shared<Tool_Magnetism>();
		_ToolType = CurrentTool;
		break;

	case ToolType::Bounce:
		_CurrentTool = std::make_shared<Tool_Bounce>();
		_ToolType = CurrentTool;
		break;

	case ToolType::Conductive:
		_CurrentTool = std::make_shared<Tool_Conductivity>();
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
	// Energy Regen
	if ( _Energy < _EnergyMax && timer.GetMilliSecondsElapsed() >= 1000 )
		_Energy += 1;
	else if ( _Energy == _EnergyMax )
	{
		timer.Stop();
		timer.Restart();
	}
}

void Tool_Class::AddToEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::ChangeToolEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ChangeToolOptionEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ChangeToolOptionDownEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ChangeToolOptionUpEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ChangeCubeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ChangeAllCubeEvent, this );
	EventSystem::Instance()->AddEvent( EVENTID::ToolModeEvent, this );
}

void Tool_Class::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeToolEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeToolOptionEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeToolOptionDownEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeToolOptionUpEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeCubeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeAllCubeEvent, this );
}

void Tool_Class::HandleEvent( Event* event )
{
	// Change tool
	switch ( event->GetEventID() )
	{
	case EVENTID::ChangeToolOptionEvent:
	{
		_CurrentTool->ChangeCurrent( *static_cast<int*>( event->GetData() ) );
	}
	break;
	case EVENTID::ChangeToolOptionUpEvent:
	{
		_CurrentTool->AddOneToCurrent();
	}
	break;
	case EVENTID::ChangeToolOptionDownEvent:
	{
		_CurrentTool->MinusOneTOCurrent();
	}
	break;
	case EVENTID::ChangeToolEvent:
	{
		SetCurrentTool( *static_cast<ToolType*>( event->GetData() ) );
	}
	break;
	case EVENTID::ChangeCubeEvent:
	{
		if ( _CurrentTool->GetToolData().MagMode != MagnetismMode::AllCubes )
		{
			timer.Restart();
			if ( _Energy > 0 )
			{
				_Energy -= _CurrentTool->GetEnergyCost();
				timer.Start();

				CubeProperties* cube = static_cast<CubeProperties*>( event->GetData() );
				ChangeCube( cube );

				Sound::Instance()->PlaySoundEffect( "ToolUse" );
			}
			else if ( _Energy <= 0 )
				Sound::Instance()->PlaySoundEffect( "ToolNoEnergy" );
		}
	}
	break;
	case EVENTID::ChangeAllCubeEvent:
	{
		timer.Restart();
		if ( _Energy > 0 )
		{
			if ( _CurrentTool->GetToolData().MagMode == MagnetismMode::AllCubes )
			{
				_Energy -= _CurrentTool->GetEnergyCost() * 2;
				// Change all cube properties
				for ( auto& cube : *static_cast<std::vector<std::shared_ptr<Cube>>*>( event->GetData() ) )
					ChangeCube( cube->GetEditableProperties().get() );
			}
		}
	}
	break;
	}
}

void Tool_Class::ChangeCube( CubeProperties* Cube )
{
	switch ( _ToolType )
	{
	case ToolType::Convert:
		Cube->SetBoxType( _CurrentTool->GetToolData().boxtype );
		break;
	case ToolType::Resize:
		Cube->SetBoxSize( _CurrentTool->GetToolData().boxSize );
		break;
	case ToolType::Magnetism:
		Cube->SetBoxMagneticMove( true );
		break;
	case ToolType::Bounce:
		Cube->SetBoxBounce( _CurrentTool->GetToolData().boxBounce );
		break;
	case ToolType::Conductive:
		if ( _CurrentTool->GetToolData().boxConductive == BoxConductive::IsConductive )
			Cube->SetConductive( true );
		else
			Cube->SetConductive( false );
		break;
	}
}