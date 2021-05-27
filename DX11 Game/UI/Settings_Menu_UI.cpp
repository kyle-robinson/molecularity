#include "stdafx.h"
#include "Settings_Menu_UI.h"

Settings_Menu_UI::Settings_Menu_UI()
{
}

Settings_Menu_UI::~Settings_Menu_UI()
{
	RemoveFromEvent();
}

void Settings_Menu_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts)
{
	UI::Inizalize(device, contex, cb_vs_matrix_2d,fonts);
	AddtoEvent();

	_isSettings = false;
	_SettingsData = JSON::LoadSettings();

	TextLoad();

	LoadFlag = true;
	//font
	FontsList->AddFont("OpenSans_50", "OpenSans_50.spritefont");
	FontsList->AddFont("OpenSans_12", "OpenSans_12.spritefont");

	//init sprites
	SettingsBackground.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	SettingsScrollBar.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	SettingsScrollBar.setPY(0);
	SettingsScrollBar.SetPageSize(_SizeOfScreen.y);
	for (unsigned int i = 0; i < 10; i++) {
		SettingsDropdowns[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		SettingsSliders[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		SettingsButtions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);

	}
	for (unsigned int i = 0; i < 20; i++) {
		ControllInput[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

}

void Settings_Menu_UI::Update(float dt)
{
	if (_isSettings) {
	
		if (mouseLoad) {
			_MouseData.LPress = false;
			mouseLoad = false;
		}
		//Bakground
		SettingsBackground.Function("Settings\\settingsBack.dds", { _SizeOfScreen.x ,_SizeOfScreen.y+10 }, { 0,0 });
		
		PageSlider();
		TabButtions();
		TabContent();
		Accept();

	
		//Headding text
		TextToDraw TextToDraw;
		TextToDraw._Colour = Colors::Black;
		TextToDraw._Text = "Settings";
		TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0),static_cast<float>(_SizeOfScreen.y * 0)- static_cast<float>(_SizeOfScreen.y * 0.03) };
		PuaseTextTitles.push_back(TextToDraw);
	}
}

void Settings_Menu_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	if (_isSettings) {
		Shaders::BindShaders(_Contex.Get(), vert, pix);
		SettingsBackground.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		SettingsScrollBar.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);

		for (UINT i = 0; i < SettingSliderCount; i++) {
			SettingsSliders[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		}
		for (UINT i = 0; i < SettingsButtionCount; i++) {
			SettingsButtions[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		
		for (int i = SettingsDropCount-1; i >= 0; i--) {
			
			SettingsDropdowns[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get(),vert ,pix);
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (UINT i = 0; i < SettingsInputCount; i++) {
			ControllInput[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (UINT i = 0; i < PuaseTextTitles.size(); i++)
		{
			FontsList->GetFont("OpenSans_50")->RenderString(PuaseTextTitles[i]._Text, PuaseTextTitles[i]._Position, PuaseTextTitles[i]._Colour);
		}

		PuaseTextTitles.clear();

		for (UINT i = 0; i < PuaseTextPG.size(); i++)
		{
			FontsList->GetFont("OpenSans_12")->RenderString(PuaseTextPG[i]._Text, PuaseTextPG[i]._Position, PuaseTextPG[i]._Colour);
		}

		PuaseTextPG.clear();

		SettingsButtionCount = 0;
		SettingsDropCount = 0;
		SettingSliderCount = 0;
		SettingsInputCount = 0;
	}
}

void Settings_Menu_UI::TextLoad()
{
	vector<JSON::TextData>ButtionsText = TextLoader::Instance()->LoadText("Settings_Buttions");
	LoadedTextMap = TextLoader::Instance()->ConvertToMap(ButtionsText);

	vector<JSON::TextData>setText = TextLoader::Instance()->LoadText("Settings_Names");
	int i=0;
	for (auto& text : _SettingsData) {
		text.Text = setText[i].Text;
		i++;
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
	
		SettingsScrollBar.SetPageSize(_SizeOfScreen.y);
		_MouseData.LPress = false;
		LoadFlag = true;
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
		TextToDraw._Text = settingData.Text;
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

void Settings_Menu_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::GameSettingsEvent, this);
}

void Settings_Menu_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameSettingsEvent, this);
}

void Settings_Menu_UI::PageSlider()
{
	
	SettingsScrollBar.Function({ 30, static_cast<float>(_SizeOfScreen.y * 0.60) }, { _SizeOfScreen.x - 30 ,static_cast<float>(_SizeOfScreen.y * 0.30) }, 0, Colour{ 0,0,0 }, Colour{ 0,0,0 }, _MouseData);
	currentY = (static_cast<float>(_SizeOfScreen.y * 0.37)) - SettingsScrollBar.getPagePos();
	if (currentPY != SettingsScrollBar.getPY()) {
		currentPY = SettingsScrollBar.getPY();
		LoadFlag = true;
	}


	//box for colision 
	boxPos = { 0,static_cast<float>(_SizeOfScreen.y * 0.30) };
	boxSize = { _SizeOfScreen.x, static_cast<float>(_SizeOfScreen.y * 0.60) };
}

void Settings_Menu_UI::TabButtions()
{
	float PosXButtion = 0;

	if (SettingsButtions[SettingsButtionCount].Function(LoadedTextMap["Buttion_1"], ButtionTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButtion ,  (float)(_SizeOfScreen.y * 0.13) }, DirectX::Colors::Black, _MouseData)) {
		CurrentTab = GenralTab;
		SettingsScrollBar.setPY(0);
	}
	PosXButtion += _SizeOfScreen.x / 14;
	SettingsButtionCount++;
	if (SettingsButtions[SettingsButtionCount].Function(LoadedTextMap["Buttion_2"], ButtionTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButtion ,  (float)(_SizeOfScreen.y * 0.13) }, DirectX::Colors::Black, _MouseData)) {
		CurrentTab = GrapicsTab;
		SettingsScrollBar.setPY(0);

	}
	PosXButtion += _SizeOfScreen.x / 14;
	SettingsButtionCount++;
	if (SettingsButtions[SettingsButtionCount].Function(LoadedTextMap["Buttion_3"], ButtionTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButtion ,  (float)(_SizeOfScreen.y * 0.13) }, DirectX::Colors::Black, _MouseData)) {
		CurrentTab = SoundTab;
		SettingsScrollBar.setPY(0);
	}
	PosXButtion += _SizeOfScreen.x / 14;
	SettingsButtionCount++;
	if (SettingsButtions[SettingsButtionCount].Function(LoadedTextMap["Buttion_4"], ButtionTex, { _SizeOfScreen.x / 14, _SizeOfScreen.y / 14 }, XMFLOAT2{ PosXButtion ,  (float)(_SizeOfScreen.y * 0.13) }, DirectX::Colors::Black, _MouseData)) {
		CurrentTab = ControlsTab;
		SettingsScrollBar.setPY(0);
	}
	SettingsButtionCount++;
}

void Settings_Menu_UI::TabContent()
{
	TextToDraw TextToDraw;
	TextToDraw._Colour = Colors::Black;
	TabTextPos = { static_cast<float>(_SizeOfScreen.x * 0.01),static_cast<float>(_SizeOfScreen.y * 0.22) };

	//swich settings tabs
	switch (CurrentTab)
	{
	case GrapicsTab:
	{

		TextToDraw._Position = TabTextPos;
		TextToDraw._Text = LoadedTextMap["Buttion_2"];
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
		TextToDraw._Text = LoadedTextMap["Buttion_1"];
		PuaseTextTitles.push_back(TextToDraw);

		
		for (auto& setting : _SettingsData)
		{
			if (setting.Type == JSON::SettingType::GeneralType)
			{
				//window size
				if (setting.Name == "WindowWidth" || setting.Name == "WindowHight") {

					WindowSizeCreate(setting);
					continue;
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
		TextToDraw._Text = LoadedTextMap["Buttion_3"];
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
		TextToDraw._Text = LoadedTextMap["Buttion_4"];
		PuaseTextTitles.push_back(TextToDraw);

		for (auto& setting : _SettingsData)
		{

			if (setting.Type == JSON::SettingType::ControllType)
			{
				if (currentY >= boxPos.y &&
					currentY <= (boxPos.y + boxSize.y)) {
					ControllsCreate(setting);
				}
				currentY += static_cast<float>(_SizeOfScreen.x * 0.05);

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
	//update file
	if (SettingsButtions[SettingsButtionCount].Function(LoadedTextMap["Buttion_Accept"], AcceptButtion, { _SizeOfScreen.x / 9, _SizeOfScreen.y / 9 }, XMFLOAT2{ static_cast<float>(_SizeOfScreen.x * 0.89) ,  static_cast<float>(_SizeOfScreen.y * 0) }, DirectX::Colors::Black, _MouseData))
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


			if (setting.Name == "Language") {
				TextLoader::Instance()->ChangeLang(std::get<string>(setting.Setting));
			}
		}



		//Notify relvent areas that settings has changed
		EventSystem::Instance()->AddEvent(EVENTID::UpdateSettingsEvent, &_SettingsData);
		

		_isSettings = false;
		CurrentTab = GenralTab;
		SettingsScrollBar.setPY(0);
		LoadFlag = true;
		mouseLoad = true;
		return;
	}
	SettingsButtionCount++;
}

void Settings_Menu_UI::ControllsCreate(JSON::SettingData& settingData)
{
	TextToDraw TextToDraw;
	TextToDraw._Colour = Colors::Black;


	TextToDraw._Colour = Colors::Black;
	TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0.01),currentY };
	TextToDraw._Text = settingData.Text;
	PuaseTextPG.push_back(TextToDraw);

	if (LoadFlag) {
		unsigned char* controll = (unsigned char*)get<string>(settingData.Setting).c_str();
		ControllInput[SettingsInputCount].SetKey(*controll);

	}
	ControllInput[SettingsInputCount].Function({ static_cast<float>(_SizeOfScreen.x * 0.15625),static_cast<float>(_SizeOfScreen.y * 0.07) }, { static_cast<float>(_SizeOfScreen.x * 0.39),currentY }, "Resources\\Textures\\Settings\\Input_Yellow.dds", DirectX::Colors::Black, Key, _MouseData);

	string output;
	output = ControllInput[SettingsInputCount].getKey();
	settingData.Setting = output;
	SettingsInputCount++;

}

void Settings_Menu_UI::WindowSizeCreate(JSON::SettingData& settingData)
{
	TextToDraw TextToDraw;
	TextToDraw._Colour = Colors::Black;
	
	if (settingData.Name == "WindowWidth") {

		if (int* input = std::get_if<int>(&settingData.Setting)) {
			curretJsonWinsize.x = *input;
		}
		return;
	}
	else if (settingData.Name == "WindowHight") {
		if (currentY >= boxPos.y &&
			currentY <= (boxPos.y + boxSize.y))
		{
			TextToDraw._Colour = Colors::Black;
			TextToDraw._Position = { static_cast<float>(_SizeOfScreen.x * 0.01),currentY };
			TextToDraw._Text = settingData.Text;
			PuaseTextPG.push_back(TextToDraw);

			if (int* input = std::get_if<int>(&settingData.Setting)) {
				vector<string> WinSize{ "1024x576","1280x720","1600x900","1920x1080","2560x1440" };
				vector<XMFLOAT2> WinSizeFlot{ {1024,576},{1280,720},{1600,900},{1920,1080},{2560,1440} };
				curretJsonWinsize.y = *input;
				int current=0;
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

				settingData.Setting = static_cast<int>(curretJsonWinsize.y);
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
			return;
		}
	}
}
