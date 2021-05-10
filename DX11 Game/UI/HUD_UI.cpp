#include "stdafx.h"
#include "HUD_UI.h"

HUD_UI::HUD_UI()
{
	EventSystem::Instance()->AddClient( EVENTID::EnergyUpdateEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ToolModeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::UpdateSettingsEvent, this );
}

HUD_UI::~HUD_UI()
{
	delete Mode;
}

void HUD_UI::Inizalize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d )
{
	EventSystem::Instance()->ProcessEvents();
	std::vector<JSON::SettingData> SettingsData = JSON::LoadSettings();

	for ( auto& setting : SettingsData )
	{
		if ( setting.Name == "Hud_Scale" ) {
			hudScale = ( float )get<int>( setting.Setting ) / 100;
		}
	}

	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;
	HudBackgrounds[0].INITSprite( contex, device, *_cb_vs_matrix_2d );
	HUDenergyWidget.INITSprite( contex, device, *_cb_vs_matrix_2d );
	for ( uint32_t i = 0; i < 3; i++ )
	{
		HUDImages[i].INITSprite( contex, device, *_cb_vs_matrix_2d );
	}

	HUDTextRenderer = make_shared<TextRenderer>( "OpenSans_12.spritefont", device, contex );
}

void HUD_UI::Update()
{
	//tool type ui change
	string TextFile;
	string ToolInformationTexture = "";

	//TODO add background and borders
	switch ( Mode->GetToolType() )
	{
	case ToolType::Convert: {
		TextFile = "HUD\\Tool_Assets\\ConvertSelect_500x500.dds";
		switch ( Mode->GetMaterialID() )
		{
		case 0: ToolInformationTexture = "crates\\basic_crate.png"; break;
		case 1: ToolInformationTexture = "crates\\bounce_crate.png"; break;
		case 2: ToolInformationTexture = "crates\\arrow_crate.png"; break;
		case 3: ToolInformationTexture = "crates\\tnt_crate.png"; break;
		}
	}
		break;
	case ToolType::Resize: {
		TextFile = "HUD\\Tool_Assets\\ReSizeSelect_500x500.dds";
		switch ( Mode->GetSizeID() )
		{
		case 0: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_Down.png"; break;
		case 1: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_Reset.png"; break;
		case 2: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_UP.png"; break;
		}
	}
		break;
	default:
		TextFile = "";
		break;
	}

	HUDImages[2].Function( ToolInformationTexture, { 500 * hudScale,500 * hudScale }, { _SizeOfScreen.x - ( 1000 * hudScale ),  _SizeOfScreen.y - ( 500 * hudScale ) } );
	HUDImages[0].Function( TextFile, { 500 * hudScale,500 * hudScale }, { _SizeOfScreen.x - ( 500 * hudScale ),  _SizeOfScreen.y - ( 500 * hudScale ) } );
	HudBackgrounds[0].Function( Colour{ 0, 0, 0,100 }, { 1000 * hudScale,500 * hudScale }, { _SizeOfScreen.x - ( 1000 * hudScale ),  _SizeOfScreen.y - ( 500 * hudScale ) }, 0.7 );
	//crosshair
	HUDImages[1].Function( "HUD\\CrossHair_Assets\\Cosshair_V2_60x60.dds", { 200 * hudScale,200 * hudScale }, { _SizeOfScreen.x / 2 - ( 200 * hudScale ) / 2,  _SizeOfScreen.y / 2 - ( 200 * hudScale ) / 2 } );
	//bar data
	HUDenergyWidget.Function( Colour{ 0,0,0 }, Colour{ 207, 164, 12,100 }, "Resources\\Textures\\HUD\\energy_Top.dds", { 1000 * hudScale,250 * hudScale }, XMFLOAT2{ 0, _SizeOfScreen.y - ( 250 * hudScale ) }, energy );
}

void HUD_UI::BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene )
{
	Shaders::BindShaders( _Contex.Get(), vert, pix );
	HudBackgrounds[0].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
	HUDenergyWidget.Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
	HUDImages[0].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );

	for ( uint32_t i = 0; i < 3; i++ )
	{
		HUDImages[i].Draw( _Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix );
	}
}

void HUD_UI::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::EnergyUpdateEvent:
	{
		energy = ( int )event->GetData();
	}
	break;
	case EVENTID::ToolModeEvent:
	{
		Mode = ( CubeProperties* )event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		_SizeOfScreen = *static_cast< XMFLOAT2* >( event->GetData() );

	}
	break;
	case EVENTID::UpdateSettingsEvent:
		std::vector<JSON::SettingData> a = *static_cast< std::vector<JSON::SettingData>* >( event->GetData() );
		for ( auto& setting : a )
		{
			if ( setting.Name == "Hud_Scale" ) {
				hudScale = ( float )get<int>( setting.Setting ) / 100;
			}
		}
	}
}