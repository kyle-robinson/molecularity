#include "stdafx.h"
#include "Custom_UI.h"
#include"Graphics.h"
#include <DirectXCollision.h>



//load all textures here
void Custom_UI::LoadTextures()
{
	//TODO JSON  FIle Loading

	//other immages

}

void Custom_UI::INITWigets()
{
	//HUD
	HUDenergyWidget.INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	HUDHealthWidget.INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 3; i++)
	{
		HUDImages[i].INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	}

	//puase
	PuaseBakgtound.INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 4; i++) {
		PuaseButtions[i].INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	}


	//settings

	SettingsBakgtound.INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	SettingsScrollBar.INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 10; i++) {
		SettingsDropdowns[i].INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
		SettingsSliders[i].INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
		SettingsButtions[i].INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
		
	}
	for (unsigned int i = 0; i < 20; i++) {
		ControllInput[i].INITSprite(_Contex.Get(), _Device.Get(), _cb_vs_matrix_2d);
	}

}

void Custom_UI::INITTexRender(Graphics* gfx)
{
	HUDTextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont",_Device.Get(),_Contex.Get());
}

Custom_UI::Custom_UI()
{
	EventSystem::Instance()->AddClient(EVENTID::Event1, this);
}
Custom_UI::~Custom_UI()
{
}
void Custom_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex)
{
	_SettingsData = JSON::LoadSettings();
	_Device = device;
	_Contex = contex;
	
	
	LoadTextures();
	/*INITWigets();*/
	INITTexRender(nullptr);
}

//draw Objects
void Custom_UI::BeginDraw(Graphics* gfx, VertexShader& vert,PixelShader& pix)
{
	Shaders::BindShaders(_Contex.Get(), vert, pix);
	//Hud

	if (!isSettings) {

		HUDenergyWidget.Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());
		HUDHealthWidget.Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());
		HUDImages[0].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());
	
		for (unsigned int i = 0; i < 3; i++)
		{
			HUDImages[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetOrthoMatrix());
		}
		for (UINT i = 0; i < HUDText.size(); i++)
		{
			HUDTextRenderer->RenderString(HUDText[i].text, HUDText[i].position, HUDText[i].colour);
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}
		HUDTextRenderer->RenderCubeMoveText(*gfx);
		Shaders::BindShaders(_Contex.Get(), vert, pix);
		HUDText.clear();
	}
	if (!isSettings&& isPaused) {
		PuaseBakgtound.Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix());
		for (unsigned int i = 0; i < 4; i++) {
			PuaseButtions[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix(), HUDTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}
	}
	if (isSettings) {
		SettingsBakgtound.Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix());
		SettingsScrollBar.Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix());

		for (UINT i = 0; i < SettingSliderCount; i++) {
			SettingsSliders[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix());
		}
		for (UINT i = 0; i < SettingsButtionCount; i++) {
			SettingsButtions[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix(), HUDTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}
		
		for (UINT i = 0; i < SettingsDropCount; i++) {
			SettingsDropdowns[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix(), HUDTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}
		
		for (UINT i = 0; i < SettingsInputCount; i++) {
			ControllInput[i].Draw(_Contex.Get(), _Device.Get(), _cb_ps_scene, _cb_vs_matrix_2d, gfx->GetCameraController()->GetUICamera().GetWorldOrthoMatrix(), HUDTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}
		
		for (UINT i = 0; i < SettingsText.size(); i++)
		{
			HUDTextRenderer->RenderString(SettingsText[i].text, SettingsText[i].position, SettingsText[i].colour);
		}

		SettingsText.clear();
		SettingsButtionCount = 0;
		SettingsDropCount = 0;
		SettingSliderCount = 0;
		SettingsInputCount = 0;
	}

	

	
}

//pre set Ui emements
void Custom_UI::MainMenu(Graphics* gfx)
{

	
}
//test values
static int energy = 100;
//game Hud
void Custom_UI::GameHUD(Graphics* gfx)
{
	
	//tool type ui change
	string TextFile;
	string ToolInformationTexture="";

	//TODO add background and borders
	switch (gfx->GetCube()[0]->GetEditableProperties()->GetToolType())
	{
	case ToolType::Convert: {
		TextFile = "HUD\\Tool_Assets\\ConvertSelect_500x500.dds";
		switch (gfx->GetCube()[0]->GetEditableProperties()->GetMaterialID())
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
		switch (gfx->GetCube()[0]->GetEditableProperties()->GetSizeID())
		{
			case 0: ToolInformationTexture = "HUD\\ResizeTool_Down.png"; break;
			case 1: ToolInformationTexture = "HUD\\ResizeTool_Reset.png"; break;
			case 2: ToolInformationTexture = "HUD\\ResizeTool_UP.png"; break;
		}
	}
	break;
	default:
		TextFile = "";
		break;
	}
	//set hud scale
	float hudScale=1;
	for (auto& setting : _SettingsData)
	{
		if (setting.Name == "Hud_Scale") {
			hudScale = (float)get<int>(setting.Setting) / 100;
		}
	}
	HUDImages[2].Function(ToolInformationTexture, { 500 * hudScale,500 * hudScale }, { (float)gfx->GetWidth() - (1000 * hudScale), (float)gfx->GetHeight()  - (500 * hudScale) });
	HUDImages[0].Function(TextFile, { 500 * hudScale,500 * hudScale }, { (float)gfx->GetWidth()-(500 * hudScale), (float)gfx->GetHeight()- (500 * hudScale) });
	//crosshair
	HUDImages[1].Function("HUD\\CrossHair_Assets\\Cosshair_V2_60x60.dds", { 200* hudScale,200* hudScale }, { (float)gfx->GetWidth() / 2 - (200 * hudScale) / 2, (float)gfx->GetHeight() / 2 - (200 * hudScale) / 2 });
	//bar data
	HUDHealthWidget.Function(Colour{ 0,0,0 }, Colour{ 255, 0, 0,100 }, "Resources\\Textures\\HUD\\Border_Top.png", { 900 * hudScale,240 * hudScale }, XMFLOAT2{ 0,(float)gfx->GetHeight() - (500 * hudScale) }, 100);
	HUDenergyWidget.Function(Colour{ 0,0,0 }, Colour{ 207, 164, 12,100 }, "Resources\\Textures\\HUD\\energy_Top.png", { 1000* hudScale,250* hudScale }, XMFLOAT2{ 0,(float)gfx->GetHeight() - (250 * hudScale) }, energy);
	
	
}

void Custom_UI::Settings(Graphics* gfx)
{
	textToDraw TextToDraw;
	//Bakground
	SettingsBakgtound.Function({ 255,255,255 }, { (float)gfx->GetWidth(),(float)gfx->GetHeight() }, { 0,0 }, 1.0f);
	//Tab Buttions
	float PosXBut = 10;
	string Names[4] = { "Genral","Grapics","Sound","Controls" };
	for (UINT i = 0; i < 4; i++)
	{
		SettingsButtions[i].Function(Names[i], vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ PosXBut ,  100 }, _MouseData);
		PosXBut += 100;
		SettingsButtionCount++;
	}

	
	//set tabs
	
	if (SettingsButtions[0].GetIsPressed()) {
		CurrentTab = GenralTab;
		
	}
	if (SettingsButtions[1].GetIsPressed()) {
		CurrentTab = GrapicsTab;
		
		
	}
	if (SettingsButtions[2].GetIsPressed()) {
		CurrentTab = SoundTab;
		
	}
	if (SettingsButtions[3].GetIsPressed()) {
		CurrentTab = ControlsTab;
		
	}
		
	//box
	XMFLOAT2 boxPos = { 0,180 };
	XMFLOAT2 boxSize = { (float)gfx->GetWidth(), (float)gfx->GetHeight() - 116 };

	//swich settings tabs
	switch (CurrentTab)
	{
		case GrapicsTab:
		{
			TextToDraw.colour = Colors::Black;
			TextToDraw.position = { 10,150 };
			TextToDraw.text = "Grapics";
			SettingsText.push_back(TextToDraw);
			float currentY = 200;
			for (auto & setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::GraphicType)
				{
					TextToDraw.colour = Colors::Black;
					TextToDraw.position = { 10,currentY };
					TextToDraw.text = setting.Name;
					SettingsText.push_back(TextToDraw);
					if (int* input = std::get_if<int>(&setting.Setting)) {

						SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
						setting.Setting = (int)SettingsSliders[SettingSliderCount].getData();
						SettingSliderCount++;


						TextToDraw.colour = Colors::Black;
						TextToDraw.position = { 750,currentY };
						TextToDraw.text = to_string(*input);
						SettingsText.push_back(TextToDraw);
					}
					currentY += 50;
				}
			}

			
			break;
		}

		case GenralTab:
		{

			TextToDraw.colour = Colors::Black;
			TextToDraw.position = { 10,150 };
			TextToDraw.text = "Genral";
			SettingsText.push_back(TextToDraw);

			float currentY = 200;

		

			for (auto & setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::GeneralType)
				{

					TextToDraw.colour = Colors::Black;
					TextToDraw.position = { 10,currentY };
					TextToDraw.text = setting.Name;
					SettingsText.push_back(TextToDraw);
					if (int* input = std::get_if<int>(&setting.Setting)) {

						SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
						setting.Setting = (int)SettingsSliders[SettingSliderCount].getData();
						SettingSliderCount++;


						TextToDraw.colour = Colors::Black;
						TextToDraw.position = { 750,currentY };
						TextToDraw.text = to_string(*input);
						SettingsText.push_back(TextToDraw);
					}
					else if (bool* input = std::get_if<bool>(&setting.Setting)) {
						if (!*input) {
							SettingsDropdowns[SettingsDropCount].setCurrent(1);
						}
						SettingsDropdowns[SettingsDropCount].Function(vector<string>{"true", "false"}, { 200,30 }, { 500,currentY }, { 0,0,0 }, _MouseData);
						if (SettingsDropdowns[SettingsDropCount].getSelected() == "false") {
							setting.Setting = false;
						}
						else
						{
							setting.Setting = true;

						}
						SettingsDropCount++;

					}
					else if (string* input = std::get_if<string>(&setting.Setting)) {

						vector<string>Language = { "Eng", "Fr","" };
						SettingsDropdowns[SettingsDropCount].Function(Language, { 200,30 }, { 500,currentY }, { 0,0,0 }, _MouseData);
						setting.Setting = SettingsDropdowns[SettingsDropCount].getSelected();
						
						
						SettingsDropCount++;

					}

					currentY += 50;
				}
			}
			break;
		}

		case SoundTab: {


			TextToDraw.colour = Colors::Black;
			TextToDraw.position = { 10,150 };
			TextToDraw.text = "Sound";
			SettingsText.push_back(TextToDraw);

			float currentY = 200;
			

			for (auto & setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::SoundType) {
					
					TextToDraw.colour = Colors::Black;
					TextToDraw.position = { 10,currentY };
					TextToDraw.text = setting.Name;
					SettingsText.push_back(TextToDraw);


					if (int* input = std::get_if<int>(&setting.Setting)) {

						SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
						setting.Setting = (int)SettingsSliders[SettingSliderCount].getData();
						SettingSliderCount++;


						TextToDraw.colour = Colors::Black;;
						TextToDraw.position = { 750,currentY };
						TextToDraw.text = to_string(*input);
						SettingsText.push_back(TextToDraw);

					}
					else if (bool* input = std::get_if<bool>(&setting.Setting)) {
						if (!*input) {
							SettingsDropdowns[SettingsDropCount].setCurrent(1);
						}
						SettingsDropdowns[SettingsDropCount].Function(vector<string>{"true", "false"}, { 200,30 }, { 500,currentY }, { 0,0,0 }, _MouseData);
					
						

						if (SettingsDropdowns[SettingsDropCount].getSelected() == "false" ) {
							setting.Setting = false;
						}
						else
						{
							setting.Setting = true;
							
						}

						


						SettingsDropCount++;
						
					}
					currentY += 50;

				}
				
			}
			break;
		}
		case ControlsTab:
		{

			TextToDraw.colour = Colors::Black;
			TextToDraw.position = { 10,150 };
			TextToDraw.text = "Controlls";
			SettingsText.push_back(TextToDraw);
			SettingsScrollBar.Function({ 30,(float)gfx->GetHeight()-100 }, { (float)gfx->GetWidth() - 30 ,150 }, 0, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
			float currentY = 180- SettingsScrollBar.getPY();
			for (auto & setting : _SettingsData)
			{

					if (setting.Type == JSON::SettingType::ControllType)
					{
						if (currentY >= boxPos.y &&
							currentY <= (boxPos.y + boxSize.y)) {

							TextToDraw.colour = Colors::Black;
							TextToDraw.position = { 10,currentY };
							TextToDraw.text = setting.Name;
							SettingsText.push_back(TextToDraw);


							string controll = get<string>(setting.Setting);
							ControllInput[SettingsInputCount].setCurrentText(controll);
							ControllInput[SettingsInputCount].Function({ 100, 30 }, { 200,currentY }, { 0,0,0 }, Key, _MouseData);
							string output = ControllInput[SettingsInputCount].getCurrentText();

							string UpperOut;
							for (UINT i = 0; i < output.size();i++) {
								UpperOut += toupper(output[i]);
							}

							setting.Setting = UpperOut;
							SettingsInputCount++;
						}
						currentY += 40;
					
					}
				
			}
			break;
		}	

	}
	
				 //update file
				 SettingsButtions[4].Function("Acccept", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ (float)gfx->GetWidth() - 100 ,  0 }, _MouseData);
				 SettingsButtionCount++;

				 if (SettingsButtions[4].GetIsPressed())
				 {


					 //update settings file
					 for (auto & setting : _SettingsData)
					 {
						 string type;
						 switch (setting.Type)
						 {
						 case JSON::SettingType::GeneralType:
							 type = "General";
							 break;
						 case JSON::SettingType::ControllType:
							 type = "Controlls";
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

						 if (bool* input = std::get_if<bool>(&setting.Setting))
						 {
							 JSON::UpdateJSONItemEX<bool>("Settings.json", type, setting.Name, *input, "");

						 }
						 else if (int* input = std::get_if<int>(&setting.Setting))
						 {
							 JSON::UpdateJSONItemEX<int>("Settings.json", type, setting.Name, *input, "");

						 }
						 else if (float* input = std::get_if<float>(&setting.Setting))
						 {
							 JSON::UpdateJSONItemEX<float>("Settings.json", type, setting.Name, *input, "");

						 }
						 else if (string* input = std::get_if<string>(&setting.Setting))
						 {
							 JSON::UpdateJSONItemEX<string>("Settings.json", type, setting.Name, *input, "");

						 }
					 }



					 //Notify relvent areas that settings has changed
					 EventSystem::Instance()->AddEvent(EVENTID::UpdateSettingsEvent)

					 
					 isSettings = false;
					 CurrentTab = GenralTab;
					 return;
				 }
	
	//Headding text
	TextToDraw.colour = Colors::Black;
	TextToDraw.position = { 100,0 };
	TextToDraw.text = "Settings";

	SettingsText.push_back(TextToDraw);
	
}

void Custom_UI::Pause(Graphics* gfx)
{
	
	if (!isSettings) {
		//bakground
		PuaseBakgtound.Function({ 0,0,0 }, { (float)gfx->GetWidth(),(float)gfx->GetHeight() }, { 0,0 }, 0.7f);

		//Buttions
		PuaseButtions[0].Function("Play", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0, 100 }, _MouseData);
		PuaseButtions[1].Function("Reset", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  200 }, _MouseData);
		PuaseButtions[2].Function("Settings", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  300 }, _MouseData);
		PuaseButtions[3].Function("Exit", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  400 }, _MouseData);
		
		if (PuaseButtions[0].GetIsPressed()) {
			isPaused = false;
		}
		else if (PuaseButtions[1].GetIsPressed()) {
			
		}
		else if (PuaseButtions[2].GetIsPressed()) {
			isSettings = true;
			
			_SettingsData = JSON::LoadSettings();
		}
		else if (PuaseButtions[3].GetIsPressed()) {
			//exit
			
		}
		
		//text

		////title
		//Text("Puase", { 100,0 }, Colors::LightGreen);
		//
		////tip
		//Text("Tip", { (float)gfx->GetWidth() / 2,(float)gfx->GetHeight() / 3 }, Colors::LightGreen);
		//Text("Tip Text", { (float)gfx->GetWidth() / 2,(float)gfx->GetHeight() / 2 }, Colors::LightGreen);

	}
	else{
		Settings(gfx);
	}
}

void Custom_UI::PreMenuItems(Graphics* gfx)
{
	
	
}





