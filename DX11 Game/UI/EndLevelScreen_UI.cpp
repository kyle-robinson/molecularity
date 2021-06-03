#include "stdafx.h"
#include "EndLevelScreen_UI.h"

EndLevelScreen_UI::EndLevelScreen_UI()
{ }

EndLevelScreen_UI::~EndLevelScreen_UI()
{
	RemoveFromEvent();
}

void EndLevelScreen_UI::Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts )
{
	AddtoEvent();
	UI::Initialize( device, contex, cb_vs_matrix_2d, fonts );

	FontsList->AddFont( "OpenSans_50", "OpenSans_50.spritefont" );
	FontsList->AddFont( "OpenSans_12", "OpenSans_12.spritefont" );
	TextLoad();

	Background.INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
	Image.INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );

	for ( uint32_t i = 0u; i < 2u; i++ )
		Buttons[i].INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
}

void EndLevelScreen_UI::Update( float dt )
{
	if ( ToShow )
	{
		_TextList.clear();
		if ( OnLoad )
		{
			EventSystem::Instance()->AddEvent( EVENTID::ShowCursorEvent );
			// Notifcation for cursor stuff
			EventSystem::Instance()->AddEvent( EVENTID::GamePauseEvent );
			OnLoad = false;
		}

		// Background
		Background.Function( { 235, 209, 240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0.0f, 0.0f }, 1.0f );

		// Title card
		Image.Function( "Title_Card\\TitleCard.png", { static_cast<float>( _SizeOfScreen.x * 0.5 ),static_cast<float>( _SizeOfScreen.y * 0.12f ) }, { ( _SizeOfScreen.x / 2.0f ) - ( static_cast<float>( _SizeOfScreen.x * 0.5f ) / 2.0f ), 0.0f } );
		AddButtons();

		TextToDraw text;
		text._Text = LoadedTextMap["Level_End_Text"];
		XMVECTOR textsize = FontsList->GetFont( "OpenSans_50" )->GetSpriteFont()->MeasureString( text._Text.c_str() );
		text._Position = { ( _SizeOfScreen.x / 2.0f ) - ( XMVectorGetX( textsize ) / 2.0f ),static_cast<float>( _SizeOfScreen.y * 0.12f ) };
		text._Colour = DirectX::Colors::Black;
		_TextList.push_back( text );
	}
}

void EndLevelScreen_UI::BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene )
{
	Shaders::BindShaders( _Contex.Get(), vert, pix );
	if ( ToShow ) {
		Background.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		Image.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		for ( uint32_t i = 0u; i < 2u; i++ )
		{
			Buttons[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont( "OpenSans_12" ).get() );
			Shaders::BindShaders( _Contex.Get(), vert, pix );
		}

		for ( auto& Text : _TextList )
			FontsList->GetFont( "OpenSans_50" )->RenderString( Text._Text, Text._Position, Text._Colour );
	}
}

void EndLevelScreen_UI::TextLoad()
{
	std::vector<JSON::TextData>EndLevel_Text = TextLoader::Instance()->LoadText( "EndLevel_Text" );
	LoadedTextMap = TextLoader::Instance()->ConvertToMap( EndLevel_Text );
	
	std::vector<JSON::TextData>EndLevelButtons_Text = TextLoader::Instance()->LoadText( "EndLevelButtons_Text" );
	std::unordered_map<std::string, std::string>tempMap = TextLoader::Instance()->ConvertToMap( EndLevelButtons_Text );
	
	// Join maps
	LoadedTextMap.insert( tempMap.begin(), tempMap.end() );
}

void EndLevelScreen_UI::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::GameEndLevelEvent:
	{
		ToShow = true;
	}
	break;
	case EVENTID::SetNextLevelEvent:
	{
		NextLevel = *static_cast<int*>( event->GetData() );
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
		_MouseData.LPress = false;
	}
	}
}

void EndLevelScreen_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UIKeyInput, this );
	EventSystem::Instance()->AddClient( EVENTID::UIMouseInput, this );
	EventSystem::Instance()->AddClient( EVENTID::SetNextLevelEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::GameEndLevelEvent, this );
}

void EndLevelScreen_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UIKeyInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UIMouseInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::SetNextLevelEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::GameEndLevelEvent, this );
}

void EndLevelScreen_UI::AddButtons()
{
	// Buttons
	if ( Buttons[0].Function( LoadedTextMap["Button_1"], ButtonTex, { _SizeOfScreen.x / 7.0f, _SizeOfScreen.y / 7.0f }, XMFLOAT2{ ( _SizeOfScreen.x / 2.0f ) - ( _SizeOfScreen.x / 7.0f ) - 50.0f,
		static_cast<float>( _SizeOfScreen.y * 0.55f ) }, DirectX::Colors::Black, _MouseData ) )
	{
		// Go to main menu
		EventSystem::Instance()->AddEvent( EVENTID::GameUnPauseEvent );
		EventSystem::Instance()->AddEvent( EVENTID::GameLevelChangeEvent, &Hub );

		ToShow = false;
		OnLoad = true;
	}
	else if ( Buttons[1].Function( LoadedTextMap["Button_2"], ButtonTex, { _SizeOfScreen.x / 7.0f, _SizeOfScreen.y / 7.0f }, XMFLOAT2{ ( _SizeOfScreen.x / 2.0f ) + 50.0f,
		static_cast<float>( _SizeOfScreen.y * 0.55f ) }, DirectX::Colors::Black, _MouseData ) )
	{
		// Go to next level
		EventSystem::Instance()->AddEvent( EVENTID::HideCursorEvent );
		EventSystem::Instance()->AddEvent( EVENTID::GameUnPauseEvent );
		EventSystem::Instance()->AddEvent( EVENTID::GameLevelChangeEvent, &NextLevel );

		ToShow = false;
		OnLoad = true;
	}
}
