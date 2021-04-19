#include "stdafx.h"
#include "HUD_UI.h"

HUD_UI::HUD_UI()
{
}

HUD_UI::~HUD_UI()
{
}

void HUD_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d)
{
	EventSystem::Instance()->AddClient(EVENTID::EnergyUpdateEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ToolUpdateEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);

	
	HUDenergyWidget.INITSprite(contex, device, *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 3; i++)
	{
		HUDImages[i].INITSprite(contex, device, *_cb_vs_matrix_2d);
	}

	HUDTextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont", device, contex);
}

void HUD_UI::Update()
{

	//tool type ui change
	string TextFile;
	string ToolInformationTexture = "";

	//TODO add background and borders
	switch (ToolSetting)
	{
	case ToolType::Convert: {
		TextFile = "HUD\\Tool_Assets\\ConvertSelect_500x500.dds";
		/*switch (gfx->GetCube()[0]->GetEditableProperties()->GetMaterialID())
		{
		case 0: ToolInformationTexture = "crates\\basic_crate.png"; break;
		case 1: ToolInformationTexture = "crates\\bounce_crate.png"; break;
		case 2: ToolInformationTexture = "crates\\arrow_crate.png"; break;
		case 3: ToolInformationTexture = "crates\\tnt_crate.png"; break;
		}*/
	}
						  break;
	case ToolType::Resize: {
		TextFile = "HUD\\Tool_Assets\\ReSizeSelect_500x500.dds";
		/*switch (gfx->GetCube()[0]->GetEditableProperties()->GetSizeID())
		{
		case 0: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_Down.png"; break;
		case 1: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_Reset.png"; break;
		case 2: ToolInformationTexture = "HUD\\Tool_Assets\\ResizeTool_UP.png"; break;
		}*/
	}
						 break;
	default:
		TextFile = "";
		break;
	}

	HUDImages[2].Function(ToolInformationTexture, { 500 * hudScale,500 * hudScale }, { _SizeOfScreen.x - (1000 * hudScale),  _SizeOfScreen.y - (500 * hudScale) });
	HUDImages[0].Function(TextFile, { 500 * hudScale,500 * hudScale }, { _SizeOfScreen.x - (500 * hudScale),  _SizeOfScreen.y - (500 * hudScale) });
	//crosshair
	HUDImages[1].Function("HUD\\CrossHair_Assets\\Cosshair_V2_60x60.dds", { 200 * hudScale,200 * hudScale }, { _SizeOfScreen.x / 2 - (200 * hudScale) / 2,  _SizeOfScreen.y / 2 - (200 * hudScale) / 2 });
	//bar data
	HUDenergyWidget.Function(Colour{ 0,0,0 }, Colour{ 207, 164, 12,100 }, "Resources\\Textures\\HUD\\energy_Top.png", { 1000 * hudScale,250 * hudScale }, XMFLOAT2{ 0, _SizeOfScreen.y - (250 * hudScale) }, energy);


}

void HUD_UI::BeginDraw(VertexShader& vert, PixelShader& pix)
{

	HUDenergyWidget.Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());
	HUDImages[0].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());

	for (unsigned int i = 0; i < 3; i++)
	{
		HUDImages[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());
	}

	HUDTextRenderer->RenderCubeMoveText(*gfx);
	Shaders::BindShaders(_Contex.Get(), vert, pix);
}

void HUD_UI::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::EnergyUpdateEvent:
	{
		energy = (int)event->GetData();
	}
	break;
	case EVENTID::ToolUpdateEvent:
	{
		ToolSetting = *(ToolType*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		_SizeOfScreen = *static_cast<XMFLOAT2*>(event->GetData());

	}
	break;
	case EVENTID::UpdateSettingsEvent:

		for (auto& setting : *static_cast<std::vector<JSON::SettingData>*>(event->GetData()))
		{
			if (setting.Name == "Hud_Scale") {
				hudScale = (float)get<int>(setting.Setting) / 100;
			}
		}


	default:
		break;
	}

}
