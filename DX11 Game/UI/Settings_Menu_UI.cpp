#include "stdafx.h"
#include "Settings_Menu_UI.h"

Settings_Menu_UI::Settings_Menu_UI()
{ }

Settings_Menu_UI::~Settings_Menu_UI()
{
	RemoveFromEvent();
}

void Settings_Menu_UI::Inizalize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts )
{
	UI::Initialize( device, contex, cb_vs_matrix_2d, fonts );
	AddtoEvent();

	_isSettings = false;
	_SettingsData = JSON::LoadSettings();

	TextLoad();
	LoadFlag = true;

	// Fonts
	FontsList->AddFont( "OpenSans_50", "OpenSans_50.spritefont" );
	FontsList->AddFont( "OpenSans_12", "OpenSans_12.spritefont" );

	// Initialize sprites
	SettingsBackground.INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
	SettingsScrollBar.INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
	SettingsScrollBar.SetPY( 0 );
	SettingsScrollBar.SetPageSize( _SizeOfScreen.y );

	for ( uint32_t i = 0u; i < 10u; i++ )
	{
		SettingsDropdowns[i].INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
		SettingsSliders[i].INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
		SettingsButtons[i].INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
	}

	for ( uint32_t i = 0u; i < 20u; i++ )
		ControlInput[i].INITSprite( _Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d );
}

void Settings_Menu_UI::Update( float dt )
{
	if ( _isSettings )
	{
		PauseTextTitles.clear();
		PauseTextPG.clear();

		SettingsButtonCount = 0;
		SettingsDropCount = 0;
		SettingSliderCount = 0;
		SettingsInputCount = 0;

		if ( mouseLoad )
		{
			_MouseData.LPress = false;
			mouseLoad = false;
		}

		// Background
		SettingsBackground.Function( "Settings\\settingsBack.dds", { _SizeOfScreen.x ,_SizeOfScreen.y + 10 }, { 0, 0 } );

		PageSlider();
		TabButtons();
		TabContent();
		Accept();

		// Heading text
		TextToDraw TextToDraw;
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Text = "Settings";
		TextToDraw._Position = { static_cast<float>( _SizeOfScreen.x * 0 ),static_cast<float>( _SizeOfScreen.y * 0 ) - static_cast<float>( _SizeOfScreen.y * 0.03 ) };
		PauseTextTitles.push_back( TextToDraw );
	}
}

void Settings_Menu_UI::BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene )
{
	if ( _isSettings )
	{
		Shaders::BindShaders( _Contex.Get(), vert, pix );
		SettingsBackground.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		SettingsScrollBar.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );

		for ( uint32_t i = 0u; i < SettingsButtonCount; i++ )
		{
			SettingsButtons[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont( "OpenSans_12" ).get() );
			Shaders::BindShaders( _Contex.Get(), vert, pix );
		}

		for ( uint32_t i = 0u; i < SettingSliderCount; i++ )
			SettingsSliders[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );

		for ( int i = SettingsDropCount - 1; i >= 0; i-- )
		{
			SettingsDropdowns[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont( "OpenSans_12" ).get(), vert, pix );
			Shaders::BindShaders( _Contex.Get(), vert, pix );
		}

		for ( uint32_t i = 0u; i < SettingsInputCount; i++ )
		{
			ControlInput[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont( "OpenSans_12" ).get() );
			Shaders::BindShaders( _Contex.Get(), vert, pix );
		}

		for ( uint32_t i = 0u; i < PauseTextTitles.size(); i++ )
			FontsList->GetFont( "OpenSans_50" )->RenderString( PauseTextTitles[i]._Text, PauseTextTitles[i]._Position, PauseTextTitles[i]._Colour );

		for ( uint32_t i = 0u; i < PauseTextPG.size(); i++ )
			FontsList->GetFont( "OpenSans_12" )->RenderString( PauseTextPG[i]._Text, PauseTextPG[i]._Position, PauseTextPG[i]._Colour );
	}
}

void Settings_Menu_UI::TextLoad()
{
	std::vector<JSON::TextData>ButtonsText = TextLoader::Instance()->LoadText( "Settings_Buttons" );
	LoadedTextMap = TextLoader::Instance()->ConvertToMap( ButtonsText );

	std::vector<JSON::TextData>setText = TextLoader::Instance()->LoadText( "Settings_Names" );
	int i = 0;
	for ( auto& text : _SettingsData )
	{
		text.Text = setText[i].Text;
		i++;
	}
}

void Settings_Menu_UI::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::GameSettingsEvent:
	{
		_isSettings = true;
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
		SettingsScrollBar.SetPageSize( _SizeOfScreen.y );
		_MouseData.LPress = false;
		LoadFlag = true;
	}
	break;
	}
}

void Settings_Menu_UI::CreateSettings( JSON::SettingData& settingData )
{
	TextToDraw TextToDraw;

	if ( currentY >= boxPos.y && currentY <= ( boxPos.y + boxSize.y ) )
	{
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = { static_cast<float>( _SizeOfScreen.x * 0.01 ), currentY };
		TextToDraw._Text = settingData.Text;
		PauseTextPG.push_back( TextToDraw );

		// Scaling number input
		if ( int* input = std::get_if<int>( &settingData.Setting ) )
		{
			MouseData Data;
			Data = _MouseData;
			for ( uint32_t i = 0u; i < SettingsDropCount; i++ )
			{
				if ( SettingsDropdowns[i].GetIsDown() )
					Data.LPress = false;
				else
					Data = _MouseData;
			}

			SettingsSliders[SettingSliderCount].Function( { static_cast<float>( _SizeOfScreen.x * 0.15625 ),static_cast<float>( _SizeOfScreen.y * 0.07 ) },
				{ static_cast<float>( _SizeOfScreen.x * 0.39 ),currentY }, *input, "Resources\\Textures\\Settings\\Slider_Line_Yellow.dds", "Resources\\Textures\\Settings\\Slider_Yellow.dds", Data );
			settingData.Setting = static_cast<int>( SettingsSliders[SettingSliderCount].GetData() );
			SettingSliderCount++;

			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { static_cast<float>( _SizeOfScreen.x * 0.58 ),currentY };
			TextToDraw._Text = std::to_string( *input );
			PauseTextPG.push_back( TextToDraw );
			Data.LPress = false;
		}
		// True/False input
		else if ( bool* input = std::get_if<bool>( &settingData.Setting ) )
		{
			std::string temp;
			if ( !*input )
				temp = "false";
			else
				temp = "true";

			SettingsDropdowns[SettingsDropCount].Function( std::vector<std::string>{"true", "false"}, { static_cast<float>( _SizeOfScreen.x * 0.15625 ),static_cast<float>( _SizeOfScreen.y * 0.05 ) },
				{ static_cast<float>( _SizeOfScreen.x * 0.39 ),currentY }, ButtonBackDrop, ButtonTexDrop, DirectX::Colors::White, temp, _MouseData );

			if ( SettingsDropdowns[SettingsDropCount].GetSelected() == "false" )
				settingData.Setting = false;
			else
				settingData.Setting = true;
			
			SettingsDropCount++;
		}
		// Language input
		else if ( std::string* input = std::get_if<std::string>( &settingData.Setting ) )
		{
			std::vector<std::string>Language = { "Eng", "Fr" };
			SettingsDropdowns[SettingsDropCount].Function( Language, { static_cast<float>( _SizeOfScreen.x * 0.15625 ),static_cast<float>( _SizeOfScreen.y * 0.05 ) },
				{ static_cast<float>( _SizeOfScreen.x * 0.39 ),currentY }, ButtonBackDrop, ButtonTexDrop, DirectX::Colors::White, *input, _MouseData );
			settingData.Setting = SettingsDropdowns[SettingsDropCount].GetSelected();
			SettingsDropCount++;
		}
	}

	currentY += static_cast<float>( _SizeOfScreen.y * 0.1 );
}

void Settings_Menu_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UIKeyInput, this );
	EventSystem::Instance()->AddClient( EVENTID::UIMouseInput, this );
	EventSystem::Instance()->AddClient( EVENTID::GameSettingsEvent, this );
}

void Settings_Menu_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UIKeyInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UIMouseInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::GameSettingsEvent, this );
}

void Settings_Menu_UI::PageSlider()
{
	SettingsScrollBar.Function( { 30, static_cast<float>( _SizeOfScreen.y * 0.60 ) }, { _SizeOfScreen.x - 30 ,static_cast<float>( _SizeOfScreen.y * 0.30 ) }, 0, Colour{ 0,0,0 }, Colour{ 0,0,0 }, _MouseData );
	currentY = ( static_cast<float>( _SizeOfScreen.y * 0.37 ) ) - SettingsScrollBar.GetPagePos();
	if ( currentPY != SettingsScrollBar.GetPY() )
	{
		currentPY = SettingsScrollBar.GetPY();
		LoadFlag = true;
	}

	// Box for collision
	boxPos = { 0,static_cast<float>( _SizeOfScreen.y * 0.30 ) };
	boxSize = { _SizeOfScreen.x, static_cast<float>( _SizeOfScreen.y * 0.60 ) };
}

void Settings_Menu_UI::TabButtons()
{
	float PosXButton = 0;

	if ( SettingsButtons[SettingsButtonCount].Function( LoadedTextMap["Button_1"], ButtonTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButton, static_cast<float>( _SizeOfScreen.y * 0.13 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		CurrentTab = GeneralTab;
		SettingsScrollBar.SetPY( 0 );
	}

	PosXButton += _SizeOfScreen.x / 14;
	SettingsButtonCount++;
	if ( SettingsButtons[SettingsButtonCount].Function( LoadedTextMap["Button_2"], ButtonTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButton, static_cast<float>( _SizeOfScreen.y * 0.13 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		CurrentTab = GraphicsTab;
		SettingsScrollBar.SetPY( 0 );
	}

	PosXButton += _SizeOfScreen.x / 14;
	SettingsButtonCount++;
	if ( SettingsButtons[SettingsButtonCount].Function( LoadedTextMap["Button_3"], ButtonTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButton, static_cast<float>( _SizeOfScreen.y * 0.13 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		CurrentTab = SoundTab;
		SettingsScrollBar.SetPY( 0 );
	}

	PosXButton += _SizeOfScreen.x / 14;
	SettingsButtonCount++;
	if ( SettingsButtons[SettingsButtonCount].Function( LoadedTextMap["Button_4"], ButtonTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButton, static_cast<float>( _SizeOfScreen.y * 0.13 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		CurrentTab = ControlsTab;
		SettingsScrollBar.SetPY( 0 );
	}

	SettingsButtonCount++;
}

void Settings_Menu_UI::TabContent()
{
	TextToDraw TextToDraw;
	TextToDraw._Colour = Colors::Black;
	TabTextPos = { static_cast<float>( _SizeOfScreen.x * 0.01 ),static_cast<float>( _SizeOfScreen.y * 0.22 ) };

	// Switch settings tabs
	switch ( CurrentTab )
	{
	case GraphicsTab:
	{
		TextToDraw._Position = TabTextPos;
		TextToDraw._Text = LoadedTextMap["Button_2"];
		PauseTextTitles.push_back( TextToDraw );

		for ( auto& setting : _SettingsData )
			if ( setting.Type == JSON::SettingType::GraphicType )
				CreateSettings( setting );

		break;
	}

	case GeneralTab:
	{
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = TabTextPos;
		TextToDraw._Text = LoadedTextMap["Button_1"];
		PauseTextTitles.push_back( TextToDraw );

		for ( auto& setting : _SettingsData )
		{
			if ( setting.Type == JSON::SettingType::GeneralType )
			{
				// Window size
				if ( setting.Name == "WindowWidth" || setting.Name == "WindowHeight" ) {

					WindowSizeCreate( setting );
					continue;
				}
				else
					CreateSettings( setting );
			}
		}
		break;
	}

	case SoundTab:
	{
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = TabTextPos;
		TextToDraw._Text = LoadedTextMap["Button_3"];
		PauseTextTitles.push_back( TextToDraw );

		for ( auto& setting : _SettingsData )
			if ( setting.Type == JSON::SettingType::SoundType )
				CreateSettings( setting );

		break;
	}
	case ControlsTab:
	{
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = TabTextPos;
		TextToDraw._Text = LoadedTextMap["Button_4"];
		PauseTextTitles.push_back( TextToDraw );

		for ( auto& setting : _SettingsData )
		{
			if ( setting.Type == JSON::SettingType::ControlType )
			{
				if ( currentY >= boxPos.y && currentY <= ( boxPos.y + boxSize.y ) )
					ControlsCreate( setting );
				currentY += static_cast<float>( _SizeOfScreen.x * 0.05 );
			}
		}
		LoadFlag = false;
		Key = 0;
		break;
	}
	}
}

void Settings_Menu_UI::Accept()
{
	// Update file
	if ( SettingsButtons[SettingsButtonCount].Function( LoadedTextMap["Button_Accept"], AcceptButton, { _SizeOfScreen.x / 9, _SizeOfScreen.y / 9 }, XMFLOAT2{ static_cast<float>( _SizeOfScreen.x * 0.89 ) ,
		static_cast<float>( _SizeOfScreen.y * 0 ) }, DirectX::Colors::Black, _MouseData ) )
	{
		// Update settings file
		for ( auto& setting : _SettingsData )
		{
			std::string type;
			switch ( setting.Type )
			{
			case JSON::SettingType::GeneralType:
				type = "General";
				break;
			case JSON::SettingType::ControlType:
				type = "Controls";
				break;
			case JSON::SettingType::SoundType:
				type = "Sound";
				break;
			case JSON::SettingType::GraphicType:
				type = "Graphics";
				break;
			default:
				type = "Invalid";
				break;
			}

			if ( bool* input = std::get_if<bool>( &setting.Setting ) )
				JSON::UpdateJSONItemEX<bool>( "Settings.json", type, setting.Name, *input, "" );
			else if ( int* input = std::get_if<int>( &setting.Setting ) )
				JSON::UpdateJSONItemEX<int>( "Settings.json", type, setting.Name, *input, "" );
			else if ( float* input = std::get_if<float>( &setting.Setting ) )
				JSON::UpdateJSONItemEX<float>( "Settings.json", type, setting.Name, *input, "" );
			else if ( std::string* input = std::get_if<std::string>( &setting.Setting ) )
				JSON::UpdateJSONItemEX<std::string>( "Settings.json", type, setting.Name, *input, "" );

			if ( setting.Name == "Language" )
				TextLoader::Instance()->ChangeLang( std::get<std::string>( setting.Setting ) );
		}

		// Notify relvent areas that settings has changed
		EventSystem::Instance()->AddEvent( EVENTID::UpdateSettingsEvent, &_SettingsData );

		_isSettings = false;
		CurrentTab = GeneralTab;
		SettingsScrollBar.SetPY( 0 );
		LoadFlag = true;
		mouseLoad = true;
		return;
	}

	SettingsButtonCount++;
}

void Settings_Menu_UI::ControlsCreate( JSON::SettingData& settingData )
{
	TextToDraw TextToDraw;
	TextToDraw._Colour = Colors::Black;
	TextToDraw._Position = { static_cast<float>( _SizeOfScreen.x * 0.01 ),currentY };
	TextToDraw._Text = settingData.Text;
	PauseTextPG.push_back( TextToDraw );

	if ( LoadFlag )
	{
		unsigned char* control = (unsigned char*)std::get<std::string>( settingData.Setting ).c_str();
		ControlInput[SettingsInputCount].SetKey( *control );
	}

	ControlInput[SettingsInputCount].Function( { static_cast<float>( _SizeOfScreen.x * 0.15625 ),static_cast<float>( _SizeOfScreen.y * 0.07 ) },
		{ static_cast<float>( _SizeOfScreen.x * 0.39 ),currentY }, "Resources\\Textures\\Settings\\Input_Yellow.dds", DirectX::Colors::Black, Key, _MouseData );

	std::string output;
	output = ControlInput[SettingsInputCount].GetKey();
	settingData.Setting = output;
	SettingsInputCount++;
}

void Settings_Menu_UI::WindowSizeCreate( JSON::SettingData& settingData )
{
	TextToDraw TextToDraw;
	TextToDraw._Colour = Colors::Black;

	if ( settingData.Name == "WindowWidth" ) {

		if ( int* input = std::get_if<int>( &settingData.Setting ) )
			curretJsonWinsize.x = *input;
		return;
	}
	else if ( settingData.Name == "WindowHeight" )
	{
		if ( currentY >= boxPos.y && currentY <= ( boxPos.y + boxSize.y ) )
		{
			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { static_cast<float>( _SizeOfScreen.x * 0.01 ),currentY };
			TextToDraw._Text = settingData.Text;
			PauseTextPG.push_back( TextToDraw );

			if ( int* input = std::get_if<int>( &settingData.Setting ) ) {
				std::vector<std::string> WinSize{ "1024x576","1280x720","1600x900","1920x1080","2560x1440" };
				std::vector<XMFLOAT2> WinSizeFloat{ {1024,576},{1280,720},{1600,900},{1920,1080},{2560,1440} };
				curretJsonWinsize.y = *input;
				int current = 0;
				for ( uint32_t i = 0u; i < WinSize.size(); i++ )
				{
					if ( curretJsonWinsize.x == WinSizeFloat[i].x && curretJsonWinsize.y == WinSizeFloat[i].y )
					{
						current = i;
						break;
					}
				}

				SettingsDropdowns[SettingsDropCount].Function( WinSize, { static_cast<float>( _SizeOfScreen.x * 0.15625 ),static_cast<float>( _SizeOfScreen.y * 0.05 ) }, { static_cast<float>( _SizeOfScreen.x * 0.39 ),currentY },
					ButtonBackDrop, ButtonTexDrop, DirectX::Colors::White, WinSize[current], _MouseData );

				curretJsonWinsize = WinSizeFloat[SettingsDropdowns[SettingsDropCount].GetIntSelcted()];
				settingData.Setting = static_cast<int>( curretJsonWinsize.y );

				for ( auto& setting : _SettingsData )
				{
					if ( setting.Name == "WindowWidth" )
					{
						setting.Setting = static_cast<int>( curretJsonWinsize.x );
						break;
					}
				}

				SettingsDropCount++;
				currentY += static_cast<float>( _SizeOfScreen.y * 0.1 );
			}

			return;
		}
	}
}