#include "stdafx.h"
#include "Main_Menu_UI.h"
#include <shellapi.h>

Main_Menu_UI::Main_Menu_UI() {}

Main_Menu_UI::~Main_Menu_UI()
{
	RemoveFromEvent();
}

void Main_Menu_UI::Inizalize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts )
{
	AddtoEvent();
	IsSettings = false;

	UI::Initialize( device, contex, cb_vs_matrix_2d, fonts );
	FontsList->AddFont( "OpenSans_12", "OpenSans_12.spritefont" );
	Titlecard.INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
	TextLoad();

	MainMenuBackground.INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
	for ( uint32_t i = 0u; i < 5u; i++ )
		MainMenuButtons[i].INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
}

void Main_Menu_UI::Update( float dt )
{
	if ( !IsSettings )
	{
		if ( mouseLoad )
		{
			mouseLoad = false;
			_MouseData.LPress = false;
			EventSystem::Instance()->AddEvent( EVENTID::GamePauseEvent );
		}

		MainMenuBackground.Function( { 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 0.7f );
		Titlecard.Function( "Title_Card\\TitleCard.png", { static_cast<float>( _SizeOfScreen.x * 0.4 ),static_cast<float>( _SizeOfScreen.y * 0.12 ) }, { static_cast<float>( ( _SizeOfScreen.x * 0.5 ) - static_cast<float>( _SizeOfScreen.x * 0.4 ) / 2 ) ,0 } );

		MenuButtons();
		LinkButtons();
	}
}

void Main_Menu_UI::BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene )
{
	if ( !IsSettings )
	{
		Shaders::BindShaders( _Contex.Get(), vert, pix );
		MainMenuBackground.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		Titlecard.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		for ( uint32_t i = 0u; i < 5u; i++ )
		{
			MainMenuButtons[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont( "OpenSans_12" ).get() );
			Shaders::BindShaders( _Contex.Get(), vert, pix );
		}
	}
}

void Main_Menu_UI::TextLoad()
{
	std::vector<JSON::TextData>Main_Menu_Text = TextLoader::Instance()->LoadText( "Main_Menu_Text" );
	LoadedTextMap = TextLoader::Instance()->ConvertToMap( Main_Menu_Text );
}

void Main_Menu_UI::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::UpdateSettingsEvent:
	{
		IsSettings = false;
	}
	break;
	case EVENTID::UIKeyInput:
	{
		Key = *(unsigned char*)event->GetData();
	}
	break;
	case EVENTID::UIMouseInput:
	{
		_MouseData = *(MouseData*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		_SizeOfScreen = *static_cast<XMFLOAT2*>( event->GetData() );
	}
	break;
	}
}

void Main_Menu_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UIKeyInput, this );
	EventSystem::Instance()->AddClient( EVENTID::UIMouseInput, this );
	EventSystem::Instance()->AddClient( EVENTID::UpdateSettingsEvent, this );
}

void Main_Menu_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UIKeyInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UIMouseInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UpdateSettingsEvent, this );
}

void Main_Menu_UI::MenuButtons()
{
	XMFLOAT2 size{ static_cast<float>( _SizeOfScreen.x * 0.15f ), static_cast<float>( _SizeOfScreen.y * 0.13f ) };
	float ButtonXPos = static_cast<float>( ( _SizeOfScreen.x * 0.5f ) - size.x / 2.0f );
	
	// Level 1
	float ButtonYPos = 0.25f;
	if ( MainMenuButtons[0].Function( LoadedTextMap["Button_1"], ButtonTex, size, XMFLOAT2{ ButtonXPos, static_cast<float>( _SizeOfScreen.y * ButtonYPos ) }, DirectX::Colors::Black, _MouseData ) )
	{
		LevelTo = 1;
		mouseLoad = true;
		EventSystem::Instance()->AddEvent( EVENTID::HideCursorEvent );
		EventSystem::Instance()->AddEvent( EVENTID::GameUnPauseEvent );
		EventSystem::Instance()->AddEvent( EVENTID::GameLevelChangeEvent, &LevelTo );
	}

	// Credits
	ButtonYPos += 0.20;
	if ( MainMenuButtons[1].Function( LoadedTextMap["Button_2"], ButtonTex, size, XMFLOAT2{ ButtonXPos,  static_cast<float>( _SizeOfScreen.y * 0.40 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		LevelTo = 4;
		EventSystem::Instance()->AddEvent( EVENTID::GameLevelChangeEvent, &LevelTo );
		mouseLoad = true;
	}

	// Settings
	ButtonYPos += 0.20;
	if ( MainMenuButtons[2].Function( LoadedTextMap["Button_3"], ButtonTex, size, XMFLOAT2{ ButtonXPos,  static_cast<float>( _SizeOfScreen.y * 0.55 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		mouseLoad = true;
		IsSettings = true;
		EventSystem::Instance()->AddEvent( EVENTID::GameSettingsEvent );
	}

	// Exit Game
	ButtonYPos += 0.20;
	if ( MainMenuButtons[3].Function( LoadedTextMap["Button_4"], ButtonTex, size, XMFLOAT2{ ButtonXPos,  static_cast<float>( _SizeOfScreen.y * 0.70 ) }, DirectX::Colors::Black, _MouseData ) )
		EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
}

void Main_Menu_UI::LinkButtons()
{
	// Link to GitHub
	if ( MainMenuButtons[4].Function( "", ButtonTex2, { static_cast<float>( _SizeOfScreen.x * 0.055 ), static_cast<float>( _SizeOfScreen.y * 0.075 ) }, XMFLOAT2{ 2,  2 }, DirectX::Colors::Black, _MouseData ) )
		if ( !openlink && open )
			openlink = true;

	if ( MainMenuButtons[4].GetIsPressed() == false )
		open = true;

	if ( openlink )
	{
		ShellExecute( 0, 0, L"https://github.com/kyle-robinson/directx-game", 0, 0, SW_SHOW );
		open = false;
		openlink = false;
	}
}