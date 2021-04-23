#include "stdafx.h"
#include "Settings_Menu_UI.h"

Settings_Menu_UI::Settings_Menu_UI()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::GAmeSettingsEvent, this);
}

Settings_Menu_UI::~Settings_Menu_UI()
{
}

void Settings_Menu_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{
	_isSettings = false;
	_SettingsData = JSON::LoadSettings();
	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;

	HeadderTextRenderer = make_shared<TextRenderer>("OpenSans_50.spritefont", _Device.Get(), _Contex.Get());
	PGTextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont", _Device.Get(), _Contex.Get());

	SettingsBakgtound.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	SettingsScrollBar.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 10; i++) {
		SettingsDropdowns[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		SettingsSliders[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		SettingsButtions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);

	}
	for (unsigned int i = 0; i < 20; i++) {
		ControllInput[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}
}

void Settings_Menu_UI::Update()
{
	if (_isSettings) {
		TextToDraw TextToDraw;
		//Bakground
		SettingsBakgtound.Function({ 255,255,255 }, { _SizeOfScreen.x ,_SizeOfScreen.y }, { 0,0 }, 1.0f);
		//Tab Buttions
		float PosXBut = 10;
		string TabNames[4] = { "Genral","Grapics","Sound","Controls" };
		for (UINT i = 0; i < 4; i++)
		{
			SettingsButtions[i].Function(TabNames[i], vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ PosXBut ,  100 }, _MouseData);
			PosXBut += 100;
			SettingsButtionCount++;
		}

	

		//set tabs
		SettingsScrollBar.Function({ 30,_SizeOfScreen.y - 100 }, { _SizeOfScreen.x - 30 ,150 }, 0, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
		if (SettingsButtions[0].GetIsPressed()) {
			CurrentTab = GenralTab;
			SettingsScrollBar.setPY(0);
		}
		if (SettingsButtions[1].GetIsPressed()) {
			CurrentTab = GrapicsTab;
			SettingsScrollBar.setPY(0);

		}
		if (SettingsButtions[2].GetIsPressed()) {
			CurrentTab = SoundTab;
			SettingsScrollBar.setPY(0);
		}
		if (SettingsButtions[3].GetIsPressed()) {
			CurrentTab = ControlsTab;
			SettingsScrollBar.setPY(0);
		}

		

		float currentY = 300 - SettingsScrollBar.getPY();
		//box
		XMFLOAT2 boxPos = { 0,300 };
		XMFLOAT2 boxSize = { _SizeOfScreen.x, _SizeOfScreen.y - 116 };

		//swich settings tabs
		switch (CurrentTab)
		{
		case GrapicsTab:
		{
			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { 10,150 };
			TextToDraw._Text = "Grapics";
			PuaseTextTitles.push_back(TextToDraw);
		
			for (auto& setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::GraphicType)
				{
					if (currentY >= boxPos.y &&
						currentY <= (boxPos.y + boxSize.y))
					{
						TextToDraw._Colour = Colors::Black;
						TextToDraw._Position = { 10,currentY };
						TextToDraw._Text = setting.Name;
						PuaseTextPG.push_back(TextToDraw);
						if (int* input = std::get_if<int>(&setting.Setting)) {

							SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
							setting.Setting = (int)SettingsSliders[SettingSliderCount].getData();
							SettingSliderCount++;


							TextToDraw._Colour = Colors::Black;
							TextToDraw._Position = { 750,currentY };
							TextToDraw._Text = to_string(*input);
							PuaseTextPG.push_back(TextToDraw);
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
					}
						currentY += 50;
					
				}
			}


			break;
		}

		case GenralTab:
		{

			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { 10,150 };
			TextToDraw._Text = "Genral";
			PuaseTextTitles.push_back(TextToDraw);

			



			for (auto& setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::GeneralType)
				{
					if (currentY >= boxPos.y &&
						currentY <= (boxPos.y + boxSize.y))
					{
						TextToDraw._Colour = Colors::Black;
						TextToDraw._Position = { 10,currentY };
						TextToDraw._Text = setting.Name;
						PuaseTextPG.push_back(TextToDraw);
						if (int* input = std::get_if<int>(&setting.Setting)) {

							SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
							setting.Setting = (int)SettingsSliders[SettingSliderCount].getData();
							SettingSliderCount++;


							TextToDraw._Colour = Colors::Black;
							TextToDraw._Position = { 750,currentY };
							TextToDraw._Text = to_string(*input);
							PuaseTextPG.push_back(TextToDraw);
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
					}
						currentY += 50;
					
				}
			}
			break;
		}

		case SoundTab: {


			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { 10,150 };
			TextToDraw._Text = "Sound";
			PuaseTextTitles.push_back(TextToDraw);



			for (auto& setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::SoundType) {
					if (currentY >= boxPos.y &&
						currentY <= (boxPos.y + boxSize.y))
					{
						TextToDraw._Colour = Colors::Black;
						TextToDraw._Position = { 10,currentY };
						TextToDraw._Text = setting.Name;
						PuaseTextPG.push_back(TextToDraw);


						if (int* input = std::get_if<int>(&setting.Setting)) {

							SettingsSliders[SettingSliderCount].Function({ 200,30 }, { 500,currentY }, *input, Colour{ 224,224,224 }, Colour{ 224,224,224 }, _MouseData);
							setting.Setting = (int)SettingsSliders[SettingSliderCount].getData();
							SettingSliderCount++;


							TextToDraw._Colour = Colors::Black;;
							TextToDraw._Position = { 750,currentY };
							TextToDraw._Text = to_string(*input);
							PuaseTextPG.push_back(TextToDraw);

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
					}
						currentY += 50;
					
				}

			}
			break;
		}
		case ControlsTab:
		{

			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { 10,150 };
			TextToDraw._Text = "Controlls";
			PuaseTextTitles.push_back(TextToDraw);
			
			for (auto& setting : _SettingsData)
			{

				if (setting.Type == JSON::SettingType::ControllType)
				{
					if (currentY >= boxPos.y &&
						currentY <= (boxPos.y + boxSize.y)) {

						TextToDraw._Colour = Colors::Black;
						TextToDraw._Position = { 10,currentY };
						TextToDraw._Text = setting.Name;
						PuaseTextPG.push_back(TextToDraw);


						string controll = get<string>(setting.Setting);
						ControllInput[SettingsInputCount].setCurrentText(controll);
						ControllInput[SettingsInputCount].Function({ 300, 30 }, { 200,currentY }, { 0,0,0 }, Key, _MouseData);
						string output = ControllInput[SettingsInputCount].getCurrentText();

						string UpperOut;
						for (UINT i = 0; i < output.size(); i++) {
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
		if (SettingsButtions[4].Function("Acccept", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ _SizeOfScreen.x - 100 ,  0 }, _MouseData))
		{


			//update settings file
			for (auto& setting : _SettingsData)
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
			EventSystem::Instance()->AddEvent(EVENTID::UpdateSettingsEvent,&_SettingsData);


			_isSettings = false;
			CurrentTab = GenralTab;
			return;
		}
		SettingsButtionCount++;
		//Headding text
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = { 100,0 };
		TextToDraw._Text = "Settings";

		PuaseTextTitles.push_back(TextToDraw);
	}
}

void Settings_Menu_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	if (_isSettings) {
		Shaders::BindShaders(_Contex.Get(), vert, pix);
		SettingsBakgtound.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		SettingsScrollBar.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);

		for (UINT i = 0; i < SettingSliderCount; i++) {
			SettingsSliders[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		}
		for (UINT i = 0; i < SettingsButtionCount; i++) {
			SettingsButtions[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, PGTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		
		for (UINT i = 0; i < SettingsDropCount; i++) {
			
			SettingsDropdowns[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, PGTextRenderer.get(),vert ,pix);
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (UINT i = 0; i < SettingsInputCount; i++) {
			ControllInput[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, PGTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (UINT i = 0; i < PuaseTextTitles.size(); i++)
		{
			HeadderTextRenderer->RenderString(PuaseTextTitles[i]._Text, PuaseTextTitles[i]._Position, PuaseTextTitles[i]._Colour);
		}

		PuaseTextTitles.clear();

		for (UINT i = 0; i < PuaseTextPG.size(); i++)
		{
			PGTextRenderer->RenderString(PuaseTextPG[i]._Text, PuaseTextPG[i]._Position, PuaseTextPG[i]._Colour);
		}

		PuaseTextPG.clear();

		SettingsButtionCount = 0;
		SettingsDropCount = 0;
		SettingSliderCount = 0;
		SettingsInputCount = 0;
	}
}

void Settings_Menu_UI::HandleEvent(Event* event)
{

	switch (event->GetEventID())
	{
	case EVENTID::GAmeSettingsEvent:
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
		_SizeOfScreen = *static_cast<XMFLOAT2*>(event->GetData());

	}
	break;

	}

}
