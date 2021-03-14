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

void Custom_UI::INITWigets(Graphics* gfx)
{
	//HUD
	HUDenergyWidget.INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	HUDHealthWidget.INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 2; i++)
	{
		HUDImages[i].INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	}

	//puase
	PuaseBakgtound.INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 4; i++) {
		PuaseButtions[i].INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	}


	//settings

	SettingsBakgtound.INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	SettingsScrollBar.INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 10; i++) {
		SettingsDropdowns[i].INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
		SettingsSliders[i].INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
		SettingsButtions[i].INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
		
	}
	for (unsigned int i = 0; i < 20; i++) {
		ControllInput[i].INITSprite(gfx->context.Get(), gfx->device.Get(), gfx->cb_vs_matrix_2d);
	}

}

void Custom_UI::INITTexRender(Graphics* gfx)
{
}

Custom_UI::Custom_UI()
{
}
Custom_UI::~Custom_UI()
{
}
void Custom_UI::Inizalize( Graphics* gfx)
{
	
	/*Device = device;
	Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;
	_cb_ps_scene = cb_ps_scene;*/
	
	
	LoadTextures();
	INITWigets(gfx);
	INITTexRender(gfx);
}

//draw Objects
void Custom_UI::BeginDraw(Graphics* gfx)
{
	Shaders::BindShaders(gfx->context.Get(), gfx->vertexShader_2D, gfx->pixelShader_2D);
	//Hud

	if (!isSettings) {
		HUDenergyWidget.Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());
		HUDHealthWidget.Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());
		for (unsigned int i = 0; i < 2; i++)
		{
			HUDImages[i].Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());
		}
	}
	if (!isSettings&& isPaused) {
		PuaseBakgtound.Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());
		for (unsigned int i = 0; i < 4; i++) {
			PuaseButtions[i].Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix(), gfx->textRenderer.get());
			Shaders::BindShaders(gfx->context.Get(), gfx->vertexShader_2D, gfx->pixelShader_2D);
		}
	}
	if (isSettings) {
		SettingsBakgtound.Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());
		SettingsScrollBar.Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());

		for (UINT i = 0; i < SettingSliderCount; i++) {
			SettingsSliders[i].Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix());
		}
		for (UINT i = 0; i < SettingsButtionCount; i++) {
			SettingsButtions[i].Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix(), gfx->textRenderer.get());
			Shaders::BindShaders(gfx->context.Get(), gfx->vertexShader_2D, gfx->pixelShader_2D);
		}
		
		for (UINT i = 0; i < SettingsDropCount; i++) {
			SettingsDropdowns[i].Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix(),gfx->textRenderer.get());
			Shaders::BindShaders(gfx->context.Get(), gfx->vertexShader_2D, gfx->pixelShader_2D);
		}
		
		for (UINT i = 0; i < SettingsInputCount; i++) {
			ControllInput[i].Draw(gfx->context.Get(), gfx->device.Get(), gfx->cb_ps_scene, gfx->cb_vs_matrix_2d, gfx->cameras->GetUICamera().GetWorldOrthoMatrix(), gfx->textRenderer.get());
			Shaders::BindShaders(gfx->context.Get(), gfx->vertexShader_2D, gfx->pixelShader_2D);
		}
		
		for (UINT i = 0; i < SettingsText.size(); i++)
		{
			gfx->textRenderer->RenderString(SettingsText[i].text, SettingsText[i].position, SettingsText[i].colour);
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
static int energy = 25;

void Custom_UI::GameHUD(Graphics* gfx)
{
	
	//tool type
	string TextFile;
	switch (gfx->GetCube().GetEditableProperties()->GetToolType())
	{
	case ToolType::Convert: {
		TextFile = "Resources\\Textures\\HUD\\Convert.png";
	}
	break;
	case ToolType::Resize: {
		TextFile = "Resources\\Textures\\HUD\\Resize.png";
	}
	break;
	default:
		TextFile = "";
		break;
	}

	HUDImages[0].Function(TextFile, { 50,50 }, { 200, (float)gfx->GetHeight() - 50 });
	HUDImages[1].Function("Resources\\Textures\\HUD\\crosshair.png", { 16,16 }, { (float)gfx->GetWidth() / 2 - 16 / 2, (float)gfx->GetHeight() / 2 - 16 / 2 });
	HUDHealthWidget.Function(Colour{ 0,0,0 }, Colour{ 255, 0, 0,100 }, "Resources\\Textures\\HUD\\Border_Top.png", { 160,50 }, XMFLOAT2{ 0,(float)gfx->GetHeight() - 100 }, 100);
	HUDenergyWidget.Function(Colour{ 0,0,0 }, Colour{ 207, 164, 12,100 }, "Resources\\Textures\\HUD\\energy_Top.png", { 200,50 }, XMFLOAT2{ 0,(float)gfx->GetHeight() - 50 }, energy);

	
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

			
			break;
		}

		case GenralTab:
		{

			TextToDraw.colour = Colors::Black;
			TextToDraw.position = { 10,150 };
			TextToDraw.text = "Genral";
			SettingsText.push_back(TextToDraw);

			float currentY = 200;

		

			for (int i = 0; i < _SettingsData.size(); i++)
			{
				if (_SettingsData[i].Type == JSON::SettingType::GenType)
				{

					TextToDraw.colour = Colors::Black;
					TextToDraw.position = { 10,currentY };
					TextToDraw.text = _SettingsData[i].Name;
					SettingsText.push_back(TextToDraw);
					if (int* input = std::get_if<int>(&_SettingsData[i].Setting)) {

						SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
						_SettingsData[i].Setting = (int)SettingsSliders[SettingSliderCount].getData();
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

		case SoundTab: {


			TextToDraw.colour = Colors::Black;
			TextToDraw.position = { 10,150 };
			TextToDraw.text = "Sound";
			SettingsText.push_back(TextToDraw);

			float currentY = 200;
			bool colou = true;

			for (int i = 0; i < _SettingsData.size(); i++) {
				if (_SettingsData[i].Type == JSON::SettingType::SoundType) {
					Colour a;
					XMVECTORF32 b;
					if (colou) {
						a = { 255,255,255 };
						b = Colors::Black;
						colou = false;
					}
					else
					{
						a = { 0,0,0 };
						b = Colors::White;
						colou = true;
					}

					TextToDraw.colour = Colors::Black;
					TextToDraw.position = { 10,currentY };
					TextToDraw.text = _SettingsData[i].Name;
					SettingsText.push_back(TextToDraw);


					if (int* input = std::get_if<int>(&_SettingsData[i].Setting)) {

						SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
						_SettingsData[i].Setting = (int)SettingsSliders[SettingSliderCount].getData();
						SettingSliderCount++;


						TextToDraw.colour = Colors::Black;;
						TextToDraw.position = { 750,currentY };
						TextToDraw.text = to_string(*input);
						SettingsText.push_back(TextToDraw);

					}
					else if (bool* input = std::get_if<bool>(&_SettingsData[i].Setting)) {
						if (!*input) {
							SettingsDropdowns[SettingsDropCount].setCurrent(1);
						}
						SettingsDropdowns[SettingsDropCount].Function(vector<string>{"true", "false"}, { 200,30 }, { 500,currentY }, { 0,0,0 }, _MouseData);
					
						

						if (SettingsDropdowns[SettingsDropCount].getSelected() == "false" ) {
							_SettingsData[i].Setting = false;
						}
						else
						{
							_SettingsData[i].Setting = true;
							
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
			for (int i = 0; i < _SettingsData.size(); i++) {

				

					if (_SettingsData[i].Type == JSON::SettingType::ControllType)
					{
						if (currentY >= boxPos.y &&
							currentY <= (boxPos.y + boxSize.y)) {

							TextToDraw.colour = Colors::Black;
							TextToDraw.position = { 10,currentY };
							TextToDraw.text = _SettingsData[i].Name;
							SettingsText.push_back(TextToDraw);


							string controll = get<string>(_SettingsData[i].Setting);
							ControllInput[SettingsInputCount].setCurrentText(controll);
							ControllInput[SettingsInputCount].Function({ 100, 30 }, { 200,currentY }, { 0,0,0 }, Key, _MouseData);
							_SettingsData[i].Setting = ControllInput[SettingsInputCount].getCurrentText();

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
					 for (JSON::SettingData setting : _SettingsData)
					 {
						 string type;
						 switch (setting.Type)
						 {
						 case JSON::SettingType::GenType:
							 type = "Genral";
							 break;
						 case JSON::SettingType::ControllType:
							 type = "Controlls";
							 break;
						 case JSON::SettingType::SoundType:
							 type = "Sound";
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
		PuaseButtions[0].Function("Play", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  100 }, _MouseData);
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


//TODO Screen Resize 

//TODO MAKE Memory mangment better;
//TODO Take out draw functions;
