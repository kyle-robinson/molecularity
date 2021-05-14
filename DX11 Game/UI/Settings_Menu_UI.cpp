#include "stdafx.h"
#include "Settings_Menu_UI.h"

Settings_Menu_UI::Settings_Menu_UI()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::GameSettingsEvent, this);
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

	//font
	HeadderTextRenderer = make_shared<TextRenderer>("OpenSans_50.spritefont", _Device.Get(), _Contex.Get());
	PGTextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont", _Device.Get(), _Contex.Get());

	//init sprites
	SettingsBakgtound.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	SettingsScrollBar.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	SettingsScrollBar.setPY(0);
	for (unsigned int i = 0; i < 10; i++) {
		SettingsDropdowns[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		SettingsSliders[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		SettingsButtions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);

	}
	for (unsigned int i = 0; i < 20; i++) {
		ControllInput[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

	CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
	HeadderTextRenderer->UpdateViewPort(newViewport);
	PGTextRenderer->UpdateViewPort(newViewport);
}

void Settings_Menu_UI::Update()
{
	if (_isSettings) {
		TextToDraw TextToDraw;
		//Bakground
		SettingsBakgtound.Function("Settings\\settingsBack.dds", { _SizeOfScreen.x ,_SizeOfScreen.y+10 }, { 0,0 });
		//Tab Buttions
		float PosXButtion = 0;
		
		
		for (UINT i = 0; i < 4; i++)
		{
			SettingsButtions[i].Function(TabNames[i], ButtionTex, { _SizeOfScreen.x/14, _SizeOfScreen.y/14 }, XMFLOAT2{ PosXButtion ,  (float)(_SizeOfScreen.y * 0.13) }, DirectX::Colors::Black, _MouseData);
			PosXButtion += _SizeOfScreen.x / 14;
			SettingsButtionCount++;
		}

	

		//set tabs
		//{ 30, static_cast<float>(_SizeOfScreen.y * 0.60) }
		SettingsScrollBar.SetPageSize(_SizeOfScreen.y);
		SettingsScrollBar.Function({ 30, static_cast<float>(_SizeOfScreen.y * 0.60) }, { _SizeOfScreen.x - 30 ,static_cast<float>(_SizeOfScreen.y * 0.30) }, 0, Colour{ 0,0,0 }, Colour{ 0,0,0 }, _MouseData);

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

		currentY = (static_cast<float>(_SizeOfScreen.y * 0.37)  )-SettingsScrollBar.getPagePos();
		if (currentPY != SettingsScrollBar.getPY()) {
			currentPY = SettingsScrollBar.getPY();
			LoadFlag = false;
		}

		TabTextPos = { static_cast<float>(_SizeOfScreen.x * 0.01),static_cast<float>(_SizeOfScreen.y * 0.22) };
		//box for colision 
		 boxPos = { 0,static_cast<float>(_SizeOfScreen.y * 0.30) };
		 boxSize = { _SizeOfScreen.x, static_cast<float>(_SizeOfScreen.y * 0.60) };
		
		//swich settings tabs
		switch (CurrentTab)
		{
		case GrapicsTab:
		{
			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = TabTextPos;
			TextToDraw._Text = "Graphics";
			PuaseTextTitles.push_back(TextToDraw);
		
			for (auto& setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::GraphicType)
				{
					CreateSettings(setting);
				}
			}


			break;
		}

		case GenralTab:
		{

			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = TabTextPos;
			TextToDraw._Text = "General";
			PuaseTextTitles.push_back(TextToDraw);
			XMFLOAT2 curretJsonWinsize = { 0,0 };
			for (auto& setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::GeneralType)
				{
					//window size
					if (setting.Name == "WindowWidth") {
						
						if (int* input = std::get_if<int>(&setting.Setting)) {
							curretJsonWinsize.x = *input;
						}
						continue;
					}
					else if (setting.Name == "WindowHight") {
						if (currentY >= boxPos.y &&
							currentY <= (boxPos.y + boxSize.y))
						{
						TextToDraw._Colour = Colors::Black;
						TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0.01),currentY };
						TextToDraw._Text = "Window Size";
						PuaseTextPG.push_back(TextToDraw);

						if (int* input = std::get_if<int>(&setting.Setting)) {
							vector<string> WinSize{ "1024x576","1280x720","1600x900","1920x1080","2560x1440" };
							vector<XMFLOAT2> WinSizeFlot{ {1024,576},{1280,720},{1600,900},{1920,1080},{2560,1440} };
							curretJsonWinsize.y = *input;
							int current;
							for (UINT i = 0; i < WinSize.size(); i++)
							{
								if (curretJsonWinsize.x == WinSizeFlot[i].x && curretJsonWinsize.y == WinSizeFlot[i].y) {
									current = i;
									break;
								}
							}


							SettingsDropdowns[SettingsDropCount].Function(WinSize, { static_cast<float>(_SizeOfScreen.x * 0.15625),static_cast<float>(_SizeOfScreen.y * 0.05) }, { static_cast<float>(_SizeOfScreen.x * 0.39),currentY },
								ButtionBackDrop, ButtionTexDrop, DirectX::Colors::White, WinSize[current], _MouseData);


							curretJsonWinsize = WinSizeFlot[SettingsDropdowns[SettingsDropCount].GetIntSelcted()];

							setting.Setting = static_cast<int>(curretJsonWinsize.y);
							for (auto& setting : _SettingsData)
							{
								if (setting.Name == "WindowWidth") {
									setting.Setting = static_cast<int>(curretJsonWinsize.x);
									break;
								}
							}
							SettingsDropCount++;
							currentY += static_cast<float>(_SizeOfScreen.y * 0.1);
							
							}
							continue;
						}
					}
					else {
						CreateSettings(setting);
					}
				}
				
			}
			break;
		}

		case SoundTab: {


			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = TabTextPos;
			TextToDraw._Text = "Sound";
			PuaseTextTitles.push_back(TextToDraw);



			for (auto& setting : _SettingsData)
			{
				if (setting.Type == JSON::SettingType::SoundType) {
					CreateSettings(setting);
				}

			}
			break;
		}
		case ControlsTab:
		{
			
			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = TabTextPos;
			TextToDraw._Text = "Controls";
			PuaseTextTitles.push_back(TextToDraw);
			
			for (auto& setting : _SettingsData)
			{

				if (setting.Type == JSON::SettingType::ControllType)
				{
					if (currentY >= boxPos.y &&
						currentY <= (boxPos.y + boxSize.y)) {

						TextToDraw._Colour = Colors::Black;
						TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0.01),currentY };
						TextToDraw._Text = setting.Name;
						PuaseTextPG.push_back(TextToDraw);

						if (!LoadFlag) {
							unsigned char* controll = (unsigned char*)get<string>(setting.Setting).c_str();
							ControllInput[SettingsInputCount].SetKey(*controll);
							
						}
						ControllInput[SettingsInputCount].Function({ static_cast<float>(_SizeOfScreen.x * 0.15625),static_cast<float>(_SizeOfScreen.y * 0.07) }, { static_cast<float>(_SizeOfScreen.x * 0.39),currentY }, "Resources\\Textures\\Settings\\Input_Yellow.dds", DirectX::Colors::Black, Key, _MouseData);
						
						string output;
						output = ControllInput[SettingsInputCount].getKey();
						setting.Setting = output;
						SettingsInputCount++;
					}
					currentY += static_cast<float>(_SizeOfScreen.x * 0.05);

				}

			}
			LoadFlag = true;
			Key = 0;
			break;
		}

		}

		//update file
		if (SettingsButtions[4].Function("Accept", AcceptButtion, { _SizeOfScreen.x / 9, _SizeOfScreen.y / 9 }, XMFLOAT2{ static_cast<float>(_SizeOfScreen.x*0.89) ,  static_cast<float>(_SizeOfScreen.y * 0) }, DirectX::Colors::Black, _MouseData))
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
			SettingsScrollBar.setPY(0);
			LoadFlag = false;
			return;
		}
		SettingsButtionCount++;
		//Headding text
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = { 10,0 };
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

		
		for (int i = SettingsDropCount-1; i >= 0; i--) {
			
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
		CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT( 0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y );
		HeadderTextRenderer->UpdateViewPort( newViewport );
		PGTextRenderer->UpdateViewPort( newViewport );
		_MouseData.LPress = false;
	}
	break;

	}

}

void Settings_Menu_UI::CreateSettings(JSON::SettingData& settingData)
{
	TextToDraw TextToDraw;
	

	if (currentY >= boxPos.y &&
		currentY <= (boxPos.y + boxSize.y))
	{
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0.01),currentY };
		TextToDraw._Text = settingData.Name;
		PuaseTextPG.push_back(TextToDraw);
		//scaling number input
		if (int* input = std::get_if<int>(&settingData.Setting)) {
			MouseData Data;
			//stop inputs for blow widgets
			//Could be improved
			Data = _MouseData;
			for (UINT i = 0; i < SettingsDropCount; i++)
			{
				if (SettingsDropdowns[i].GetIsDown()) {

					Data.LPress = false;
				}
				else
				{
					Data = _MouseData;
				}
			}
			SettingsSliders[SettingSliderCount].Function({ static_cast<float>(_SizeOfScreen.x * 0.15625),static_cast<float>(_SizeOfScreen.y * 0.07) }, { static_cast<float>(_SizeOfScreen.x * 0.39),currentY }, *input, "Resources\\Textures\\Settings\\Slider_Line_Yellow.dds", "Resources\\Textures\\Settings\\Slider_Yellow.dds", Data);
			settingData.Setting = (int)SettingsSliders[SettingSliderCount].getData();
			SettingSliderCount++;


			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0.58),currentY };
			TextToDraw._Text = to_string(*input);
			PuaseTextPG.push_back(TextToDraw);
			Data.LPress = false;
		}
		//true false input
		else if (bool* input = std::get_if<bool>(&settingData.Setting)) {
			string a;
			if (!*input) {
				a = "false";
			}
			else
			{
				a = "true";
			}
			SettingsDropdowns[SettingsDropCount].Function(vector<string>{"true", "false"}, { static_cast<float>(_SizeOfScreen.x * 0.15625),static_cast<float>(_SizeOfScreen.y * 0.05) }, { static_cast<float>(_SizeOfScreen.x * 0.39),currentY }, ButtionBackDrop, ButtionTexDrop, DirectX::Colors::White, a, _MouseData);
			
			if (SettingsDropdowns[SettingsDropCount].getSelected() == "false") {
				settingData.Setting = false;
			}
			else
			{
				settingData.Setting = true;
			}
			SettingsDropCount++;

		}
		//language input
		else if (string* input = std::get_if<string>(&settingData.Setting)) {

			vector<string>Language = { "Eng", "Fr"};
			SettingsDropdowns[SettingsDropCount].Function(Language, { static_cast<float>(_SizeOfScreen.x * 0.15625),static_cast<float>(_SizeOfScreen.y * 0.05) }, { static_cast<float>(_SizeOfScreen.x * 0.39),currentY }, ButtionBackDrop, ButtionTexDrop, DirectX::Colors::White, *input, _MouseData);
			settingData.Setting = SettingsDropdowns[SettingsDropCount].getSelected();
			SettingsDropCount++;
		}
	}
	currentY += static_cast<float>(_SizeOfScreen.y * 0.1);
}
