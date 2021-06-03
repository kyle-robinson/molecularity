#include "stdafx.h"
#include "Pause.h"
#include "HUD_UI.h"
#include "Graphics.h"
#include "UI_Manager.h"
#include "Settings_Menu_UI.h"

UI_Manager::UI_Manager()
{
	AddtoEvent();
}

void UI_Manager::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d )
{
	if ( FontsList == nullptr )
	{
		FontsList = std::make_shared<Fonts>();
		FontsList->Initialize( device, context );
	}

	for ( auto const& UIItem : UiList )
	{
		UIItem.second->SetSizeOfScreen( WinSize );
		UIItem.second->Inizalize( device, context, _cb_vs_matrix_2d, FontsList );
	}
}

UI_Manager::~UI_Manager()
{
	RemoveAllUI();
	RemoveFromEvent();
}

void UI_Manager::Update( float dt )
{
	for ( auto const& UIItem : UiList )
	{
		bool ToDraw = false;
		for ( int i = 0; i < UiToDraw.size(); i++ )
			if ( UIItem.first == UiToDraw[i] )
				ToDraw = true;

		if ( ToDraw )
			UIItem.second->Update( dt );
	}
}

void UI_Manager::Draw( VertexShader& vert, PixelShader& pix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene )
{
	for ( auto const& UIItem : UiList )
	{
		bool ToDraw = false;
		for ( int i = 0; i < UiToDraw.size(); i++ )
			if ( UIItem.first == UiToDraw[i] )
				ToDraw = true;

		if ( ToDraw )
			UIItem.second->BeginDraw( vert, pix, XMLoadFloat4x4( &WorldOrthMatrix ), _cb_ps_scene );
	}
}

std::shared_ptr<UI> UI_Manager::GetCustomUi( std::string UIName )
{
	for ( auto const& UIItem : UiList )
		if ( UIItem.first == UIName )
			return UiList[UIName];

	return nullptr;
}

void UI_Manager::AddUi( std::shared_ptr < UI> NewUI, std::string Name )
{
	// Check if it is in list
	bool ToAdd = true;
	for ( auto const& UIItem : UiList )
	{
		if ( UIItem.first == Name )
		{
			ToAdd = false;
			break;
		}
	}
	if ( ToAdd )
	{
		UiList[Name] = NewUI;
		UiToDraw.push_back( Name );
	}
}

void UI_Manager::RemoveUI( std::string Name )
{
	for ( auto i = UiList.begin(); i != UiList.end(); i++ )
	{
		if ( i->first == Name )
		{
			// Delete the client in question. No need to keep going since its a unique and more cant exist
			i = UiList.erase( i );
			break;
		}
	}
}

void UI_Manager::RemoveAllUI()
{
	UiList.clear();
}

void UI_Manager::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::WorldOrthMatrixEvent:
	{
		WorldOrthMatrix = *(XMFLOAT4X4*)event->GetData();
	}
	break;

	case EVENTID::WindowSizeChangeEvent:
	{
		WinSize = *static_cast<XMFLOAT2*>( event->GetData() );
		FontsList->ResizeViewAll( WinSize );
	}
	break;

	case EVENTID::RemoveUIItemEvent:
	{
		RemoveUI( *static_cast<std::string*>( event->GetData() ) );
	}
	break;

	case EVENTID::ChangeLanguageEvent:
	{
		for ( auto const& UIItem : UiList )
			UIItem.second->TextLoad();
	}
	break;

	case EVENTID::UpdateSettingsEvent:
	{
		std::vector<JSON::SettingData> SettingsData = *static_cast<std::vector<JSON::SettingData>*>( event->GetData() );
		for ( auto& setting : SettingsData )
			if ( setting.Name == "Language" )
				TextLoader::Instance()->ChangeLang( std::get<std::string>( setting.Setting ) );
	}
	break;
	}
}

void UI_Manager::AddtoEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::WorldOrthMatrixEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::RemoveUIItemEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ChangeLanguageEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UpdateSettingsEvent, this );
}

void UI_Manager::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::WorldOrthMatrixEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RemoveUIItemEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ChangeLanguageEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UpdateSettingsEvent, this );
}

void UI_Manager::HideAllUI()
{
	for ( auto const& UIItem : UiList )
	{
		std::vector<std::string>::iterator iter = UiToDraw.begin();
		while ( iter != UiToDraw.end() )
		{
			if ( *iter == UIItem.first )
				iter = UiToDraw.erase( iter );
			else
				++iter;
		}
	}
}

void UI_Manager::ShowAllUi()
{
	for ( auto const& UIItem : UiList )
		UiToDraw.push_back( UIItem.first );
}

void UI_Manager::ShowUi( std::string Name )
{
	UiToDraw.push_back( Name );
}

void UI_Manager::HideUi( std::string Name )
{
	std::vector<std::string>::iterator iter = UiToDraw.begin();
	while ( iter != UiToDraw.end() )
	{
		if ( *iter == Name )
			iter = UiToDraw.erase( iter );
		else
			++iter;
	}
}