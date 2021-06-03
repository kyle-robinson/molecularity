#include "stdafx.h"
#include "HUD_UI.h"

HUD_UI::HUD_UI()
{
	LastKey = "_";
	key = "_";
}

HUD_UI::~HUD_UI()
{
	RemoveFromEvent();
}

void HUD_UI::Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts )
{
	LastKey = "_";
	key = "_";
	AddtoEvent();
	TextLoad();
	std::vector<JSON::SettingData> SettingsData = JSON::LoadSettings();
	UpdateSettingsData( SettingsData );

	UI::Initialize( device, contex, cb_vs_matrix_2d, fonts );
	HudBakgrounds[0].INITSprite( contex, device, *_cb_vs_matrix_2d );
	HUDenergyWidget.INITSprite( contex, device, *_cb_vs_matrix_2d );
	for ( uint32_t i = 0; i < 3; i++ )
		HUDImages[i].INITSprite( contex, device, *_cb_vs_matrix_2d );

	FontsList->AddFont( "OpenSans_Bold_14", "OpenSans_Bold_14.spritefont" );
}

void HUD_UI::Update( float dt )
{
	// Tool type UI change
	std::string TextFile;
	std::string ToolInformationTexture = "";
	CreateToolHud();

	// Crosshair
	HUDImages[1].Function( "HUD\\CrossHair_Assets\\Cosshair_V2_60x60.dds", { 200 * hudScale,200 * hudScale }, { _SizeOfScreen.x / 2 - ( 200 * hudScale ) / 2,  _SizeOfScreen.y / 2 - ( 200 * hudScale ) / 2 } );

	// Cube pickup text
	PickupText._Colour = Colors::White;
	if ( !isDissCube )
		PickupText._Text = LoadedTextMap["Action_Text"];
	else
		PickupText._Text = LoadedTextMap["Action_Text2"];

	// Center text
	XMVECTOR textsize = FontsList->GetFont( "OpenSans_Bold_14" )->GetSpriteFont()->MeasureString( PickupText._Text.c_str() );
	PickupText._Position = { ( _SizeOfScreen.x * 0.5f ) - ( ( XMVectorGetX( textsize ) * FontsList->GetFont( "OpenSans_Bold_14" )->GetScale().x ) / 2 ), _SizeOfScreen.y * 0.55f };
}

void HUD_UI::BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene )
{
	if ( isHudActive )
	{
		Shaders::BindShaders( _Contex.Get(), vert, pix );
		HudBakgrounds[0].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		HUDenergyWidget.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
		HUDImages[0].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );

		for ( uint32_t i = 0; i < 3; i++ )
			HUDImages[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );

		if ( canHoldCube )
			FontsList->GetFont( "OpenSans_Bold_14" )->RenderString( PickupText._Text, PickupText._Position, PickupText._Colour );
	}
}

void HUD_UI::TextLoad()
{
	std::vector<JSON::TextData> LoadedTextData = TextLoader::Instance()->LoadText( "HUD_Text" );
	LoadedTextMap = TextLoader::Instance()->ConvertToMap( LoadedTextData );
	UpdateKeytext();
}

void HUD_UI::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::CubePickupEvent:
	{
		canHoldCube = static_cast<bool>( event->GetData() );
	}
	break;
	case EVENTID::ToolModeEvent:
	{
		Mode = (Tool_Class*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		_SizeOfScreen = *static_cast<XMFLOAT2*>( event->GetData() );
	}
	break;
	case EVENTID::UpdateSettingsEvent:
	{
		std::vector<JSON::SettingData> SettingsData = *static_cast<std::vector<JSON::SettingData>*>( event->GetData() );
		UpdateSettingsData( SettingsData );
	}
	break;
	case EVENTID::IsDissCube:
	{
		isDissCube = *static_cast<bool*>( event->GetData() );
	}
	break;
	}
}

void HUD_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::CubePickupEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ToolModeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UpdateSettingsEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::IsDissCube, this );
}

void HUD_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::CubePickupEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ToolModeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::UpdateSettingsEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::IsDissCube, this );
}

void HUD_UI::CreateToolHud()
{
	// Tool type UI change
	std::string TextFile;
	std::string ToolInformationTexture = "";

	switch ( Mode->GetTooltype() )
	{
	case ToolType::Convert: {
		TextFile = "HUD\\Tool_Assets\\ConvertSelect_500x500.dds";
		switch ( static_cast<int>( Mode->GetCurrentOption().boxtype ) )
		{
		case 0: ToolInformationTexture = "crates\\mesh.png"; break;
		case 1: ToolInformationTexture = "crates\\wood.png"; break;
		case 2: ToolInformationTexture = "crates\\iron.jpg"; break;
		case 3: ToolInformationTexture = "crates\\dCube.png"; break;
		}
	}
						  break;
	case ToolType::Resize: {
		TextFile = "HUD\\Tool_Assets\\ReSizeSelect_500x500.dds";
		switch ( static_cast<int>( Mode->GetCurrentOption().boxSize ) )
		{
		case 0: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_Down.png"; break;
		case 1: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_Reset.png"; break;
		case 2: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_UP.png"; break;
		}
	}
						 break;
	case ToolType::Magnetism: {
		TextFile = "HUD\\Tool_Assets\\Mag_500x500.dds";
		switch ( static_cast<int>( Mode->GetCurrentOption().MagMode ) )
		{
		case 0: ToolInformationTexture = "HUD\\Tool_Assets\\MagAll.png"; break;
		case 1: ToolInformationTexture = "HUD\\Tool_Assets\\MagOne.png"; break;
		}
	}
							break;
	case ToolType::Bounce:
	{
		TextFile = "HUD\\Tool_Assets\\Bounce.png";
		switch ( static_cast<int>( Mode->GetCurrentOption().boxBounce ) )
		{
		case 0: ToolInformationTexture = "HUD\\Tool_Assets\\Bounce_Off.png"; break;
		case 1: ToolInformationTexture = "HUD\\Tool_Assets\\Bounce_On.png"; break;
		}
	}
	break;
	case ToolType::Conductive:
	{
		TextFile = "HUD\\Tool_Assets\\Conductivity.png";
		switch ( static_cast<int>( Mode->GetCurrentOption().boxConductive ) )
		{
		case 0: ToolInformationTexture = "HUD\\Tool_Assets\\Conductivity_Off..png"; break;
		case 1: ToolInformationTexture = "HUD\\Tool_Assets\\Conductivity_On.png"; break;
		}
	}
	break;
	default:
		TextFile = "";
		ToolInformationTexture = "";
		break;
	}

	HUDImages[2].Function( ToolInformationTexture, { 500 * hudScale,500 * hudScale }, { _SizeOfScreen.x - ( 1000 * hudScale ),  _SizeOfScreen.y - ( 500 * hudScale ) } );
	HUDImages[0].Function( TextFile, { 500 * hudScale,500 * hudScale }, { _SizeOfScreen.x - ( 500 * hudScale ),  _SizeOfScreen.y - ( 500 * hudScale ) } );
	HudBakgrounds[0].Function( Colour{ 0, 0, 0,100 }, { 1000 * hudScale,500 * hudScale }, { _SizeOfScreen.x - ( 1000 * hudScale ),  _SizeOfScreen.y - ( 500 * hudScale ) }, 0.7 );

	// Bar data
	HUDenergyWidget.Function( Colour{ 0,0,0 }, Colour{ 207, 164, 12 }, "Resources/Textures/HUD/energy_Top.dds", { 1000 * hudScale,250 * hudScale }, XMFLOAT2{ 0, _SizeOfScreen.y - ( 250 * hudScale ) }, Mode->GetEnergy() );
}

void HUD_UI::UpdateKeytext()
{
	int position = LoadedTextMap["Action_Text"].find_first_of( LastKey );
	if ( position > 0 )
	{
		LoadedTextMap["Action_Text"].erase( LoadedTextMap["Action_Text"].begin() + position );
		LoadedTextMap["Action_Text"] = LoadedTextMap["Action_Text"].insert( position, key );
	}
}

void HUD_UI::UpdateSettingsData( std::vector<JSON::SettingData>& SettingsData )
{
	for ( auto& setting : SettingsData )
	{
		if ( setting.Name == "Hud_Scale" )
			hudScale = static_cast<float>( std::get<int>( setting.Setting ) ) / 100.0f;

		if ( setting.Name == "HUD_ON" )
			isHudActive = std::get<bool>( setting.Setting );

		if ( setting.Name == "Action" )
		{
			key = std::get<std::string>( setting.Setting );
			unsigned char* valChar = (unsigned char*)key.c_str();
			key = ConvertFromUnsignedCharToString( *valChar );
			UpdateKeytext();
		}
	}
}