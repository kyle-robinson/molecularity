#include "stdafx.h"
#include "Tutorial_UI.h"

Tutorial_UI::Tutorial_UI()
{
}

Tutorial_UI::~Tutorial_UI()
{
	RemoveFromEvent();
}

void Tutorial_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts)
{
	AddtoEvent();
	UI::Inizalize(device, contex, cb_vs_matrix_2d, fonts);
	//get key binds
	std::vector<JSON::SettingData> SettingsData = JSON::LoadSettings();
	LoadKeyBinds(SettingsData);

	TextLoad();


	FontsList->AddFont("OpenSans_12", "OpenSans_12.spritefont");

	TextBackground.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	

	//inital sate
	CurrentState = TutorialState::GameTut;
	stateNo = 0;

	timer.Start();
}

void Tutorial_UI::Update(float dt)
{
	
	//inital position
	yPos=0;
	xpos = _SizeOfScreen.x - static_cast<float>(_SizeOfScreen.x * 0.25);
	textSizeY = 0;
	
	
	switch (CurrentState)
	{
	case TutorialState::GameTut:
	{
		GenralTutorialText();
	}
		break;
	case TutorialState::MovementTut:
		MovementTutorialText();
		break;
	case TutorialState::ToolTut:
		ToolTutorialText();
		break;
	case TutorialState::ToolTut2:
		ToolTutorialText();
		break;
	case TutorialState::OtherTut:
		OtherTutorialText();
		break;
	default:
		break;
	}
  
	MoveToNextPannle();
	
	//end Tutorial
	if (stateNo > 4) {
		//end
		EventSystem::Instance()->AddEvent(EVENTID::UITutorialEnd);
		EventSystem::Instance()->AddEvent(EVENTID::RemoveUIItemEvent,&name);
		timer.Stop();
	}

	TextBackground.Function({ 225,225,225 }, { textSizeX,textSizeY }, { _SizeOfScreen.x - textSizeX,0 }, 0.8f);
}

void Tutorial_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	//isDraw to stop from drawing over Pause/settings
	if (IsDraw) {
		Shaders::BindShaders(_Contex.Get(), vert, pix);
		TextBackground.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);

		for (auto& Text : _TextList)
		{
			FontsList->GetFont("OpenSans_12")->RenderString(Text._Text, Text._Position, Text._Colour);
		}

		_TextList.clear();

	}
}

void Tutorial_UI::TextLoad()
{
	vector<JSON::TextData>Gen_Text = TextLoader::Instance()->LoadText("Gen_Text");
	LoadedTextMap = TextLoader::Instance()->ConvertToMap(Gen_Text);
	vector<JSON::TextData>Movement_Controls = TextLoader::Instance()->LoadText("Movement_Controls");
	map<string, string>temp = TextLoader::Instance()->ConvertToMap(Movement_Controls);
	LoadedTextMap.insert(temp.begin(), temp.end());
	vector<JSON::TextData>TOOL_TIPS = TextLoader::Instance()->LoadText("TOOL_TIPS");
	temp = TextLoader::Instance()->ConvertToMap(TOOL_TIPS);
	LoadedTextMap.insert(temp.begin(), temp.end());
	vector<JSON::TextData>TooL_Controls = TextLoader::Instance()->LoadText("Tool_Controls");
	temp = TextLoader::Instance()->ConvertToMap(TooL_Controls);
	LoadedTextMap.insert(temp.begin(), temp.end());
	vector<JSON::TextData>Other_Controls = TextLoader::Instance()->LoadText("Other_Controls");
	temp = TextLoader::Instance()->ConvertToMap(Other_Controls);
	LoadedTextMap.insert(temp.begin(), temp.end());
	vector<JSON::TextData>tip_opt = TextLoader::Instance()->LoadText("TOOL_TIPS_Options");
	temp = TextLoader::Instance()->ConvertToMap(tip_opt);
	LoadedTextMap.insert(temp.begin(), temp.end());
}

void Tutorial_UI::HandleEvent(Event* event)
{
	
	
	switch (event->GetEventID())
	{
	case EVENTID::GamePauseEvent:
	{
		IsDraw = false;
	}
	break;
	case EVENTID::GameUnPauseEvent:
	{
		IsDraw = true;
	}
	break;
	case EVENTID::UpdateSettingsEvent:
	{
		std::vector<JSON::SettingData> SettingsData = *static_cast<std::vector<JSON::SettingData>*>(event->GetData());

		LoadKeyBinds(SettingsData);
		textSizeX = 0;
		textSizeY = 0;
	}
	break;
	case EVENTID::ToolModeEvent:
	{
		Mode = (Tool_Class*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		_SizeOfScreen = *static_cast<XMFLOAT2*>(event->GetData());
		
	}
	break;
	}
	
}

void Tutorial_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ToolModeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GameUnPauseEvent, this);
}

void Tutorial_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ToolModeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateSettingsEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameUnPauseEvent, this);
}



void Tutorial_UI::GenralTutorialText()
{
	XMVECTOR textsize;
	TextToDraw text;
	text._Colour = DirectX::Colors::Black;
	//get to exsit
	text._Text = LoadedTextMap["Start_Text"];
	SetTextPos(text);

	
	_TextList.push_back(text);
}

void Tutorial_UI::MovementTutorialText()
{
	TextToDraw text;
	XMVECTOR textsize;
	text._Colour = DirectX::Colors::Black;
	//movement
	text._Text = LoadedTextMap["Movement_Title"];
	text._Text += "\n" + KeyBinds["Forward"] + LoadedTextMap["Control_1"];
	text._Text += "\n" + KeyBinds["Left"] + LoadedTextMap["Control_2"];
	text._Text += "\n" + KeyBinds["Back"] + LoadedTextMap["Control_3"];
	text._Text += "\n" + KeyBinds["Right"] + LoadedTextMap["Control_4"];
	text._Text += "\n" + KeyBinds["Jump"] + LoadedTextMap["Control_5"];
	text._Text += "\n"+ LoadedTextMap["Control_6"];
	SetTextPos(text);
	_TextList.push_back(text);




}

void Tutorial_UI::ToolTutorialText()
{

	
	TextToDraw text;
	text._Colour = DirectX::Colors::Black;
	//tool instructions 
	if (CurrentState == TutorialState::ToolTut)
	{
		text._Text = LoadedTextMap["Controls_Title"];
		SetTextPos(text);
		_TextList.push_back(text);

		
		//genral information
		text._Text = LoadedTextMap["Information"];
		SetTextPos(text);
		_TextList.push_back(text);

		//tool controls 
		
		text._Text = LoadedTextMap["Tool_Mode_Switch"];
		SetTextPos(text);
		_TextList.push_back(text);

		
		text._Text = KeyBinds["Gun_State_One"] + LoadedTextMap["Tool_1"];
		text._Text += "\n" + KeyBinds["Gun_State_Two"] + LoadedTextMap["Tool_2"];
		text._Text += "\n" + KeyBinds["Gun_State_Three"] + LoadedTextMap["Tool_3"];
		text._Text += "\n" + KeyBinds["Gun_State_Four"] + LoadedTextMap["Tool_4"];
		text._Text += "\n" + KeyBinds["Gun_State_Five"] + LoadedTextMap["Tool_5"];
		SetTextPos(text);
		_TextList.push_back(text);

	}
	//indvudual tool information
	else if (CurrentState == TutorialState::ToolTut2)
	{
		//tool 1
		text._Text = LoadedTextMap["information_Title"];
		SetTextPos(text);
		_TextList.push_back(text);

	
	//tell user about current mode
	switch (Mode->GetTooltype())
	{
	case ToolType::Convert: {
		
		text._Text = LoadedTextMap["Convert_Tool"];

		switch (static_cast<int>(Mode->GetCurrentOption().boxtype))
		{
		case 0:
			text._Text += LoadedTextMap["Convert_1"];
			break;
		case 1:
			text._Text += LoadedTextMap["Convert_2"];
			break;
		case 2:
			text._Text += LoadedTextMap["Convert_3"];
			break;
		}

		SetTextPos(text);
		_TextList.push_back(text);
	}
						  break;
	case ToolType::Resize: {
		//tool 2

		text._Text = LoadedTextMap["Size_Tool"];


		SetTextPos(text);

		_TextList.push_back(text);

		switch (static_cast<int>(Mode->GetCurrentOption().boxSize))
		{
		case 0:

			text._Text = LoadedTextMap["Size_1"];

			break;
		case 1:
			text._Text = LoadedTextMap["Size_2"];
			break;
		case 2:
			text._Text = LoadedTextMap["Size_3"];

			break;
		}

		SetTextPos(text);
		_TextList.push_back(text);
	}
	break;
	case ToolType::Bounce: {
		//tool 3
		text._Text = LoadedTextMap["Bounce_Tool"];
		SetTextPos(text);
		_TextList.push_back(text);

		
		switch (static_cast<int>(Mode->GetCurrentOption().boxBounce))
		{
		case 0:

			text._Text = LoadedTextMap["Bounce_1"];

			break;
		case 1:
			text._Text = LoadedTextMap["Bounce_2"];
			break;

		}

		SetTextPos(text);
		_TextList.push_back(text);

	}
	break;
	case ToolType::Magnetism: {
		//tool 4

		text._Text = LoadedTextMap["Magnatism_Tool"];
		SetTextPos(text);
		_TextList.push_back(text);

		
		switch (static_cast<int>(Mode->GetCurrentOption().MagMode))
		{
		case 0:

			text._Text = LoadedTextMap["Magnatism_1"];

			break;
		case 1:
			text._Text = LoadedTextMap["Magnatism_2"];
			break;
		
		}
		SetTextPos(text);
		_TextList.push_back(text);
	}
	 break;
	case ToolType::Conductive: {
		//tool 4

		text._Text = LoadedTextMap["Conductivity_Tool"];
		SetTextPos(text);
		_TextList.push_back(text);


		switch (static_cast<int>(Mode->GetCurrentOption().boxConductive))
		{
		case 0:

			text._Text = LoadedTextMap["Conductivity_1"];

			break;
		case 1:
			text._Text = LoadedTextMap["Conductivity_2"];
			break;

		}
		SetTextPos(text);
		_TextList.push_back(text);
	}
							break;

	}
	}
}

void Tutorial_UI::OtherTutorialText()
{
	TextToDraw text;

	text._Colour = DirectX::Colors::Black;
	text._Text = LoadedTextMap["Other_Title"];
	text._Text += "\n" + KeyBinds["Pause"] + LoadedTextMap["Other_Control_1"];
	text._Text += "\n" + KeyBinds["Change_Gun_State_Up"] + LoadedTextMap["Other_Control_2"];
	text._Text += "\n" + KeyBinds["Change_Gun_State_Down"] + LoadedTextMap["Other_Control_3"];
	text._Text += "\n"+ LoadedTextMap["Other_Control_4"];

	SetTextPos(text);
	_TextList.push_back(text);
}

void Tutorial_UI::MoveToNextPannle()
{
	//wait time 
	TextToDraw text;
	text._Colour = DirectX::Colors::Black;
	if (timer.GetMilliSecondsElapsed() >= 5000 && stateNo < 5) {


		text._Text = LoadedTextMap["Move_Next_Text"];
		SetTextPos(text);
		
		

		_TextList.push_back(text);
		//get is enter key down
		if ((1 << 15) & GetAsyncKeyState(VK_RETURN)) {
			//move next state
			stateNo++;
			CurrentState = static_cast<TutorialState>(stateNo);
			timer.Restart();
			textSizeX = 0;
			textSizeY = 0;
			Sound::Instance()->PlaySoundEffect( "Notification" );
		}

	}
}

void Tutorial_UI::LoadKeyBinds(std::vector<JSON::SettingData> SettingsData)
{
	

	for (auto& setting : SettingsData)
	{
		if (setting.Type == JSON::SettingType::ControlType)
		{

			string key = std::get<string>(setting.Setting);
			unsigned char* valChar = (unsigned char*)key.c_str();
			KeyBinds[setting.Name] = ConvertFromUnsignedCharTostring(*valChar);

		}
	}

}

void Tutorial_UI::SetTextPos(TextToDraw& text)
{
	XMVECTOR textsize = FontsList->GetFont("OpenSans_12")->GetSpriteFont()->MeasureString(text._Text.c_str());
	
	if (textSizeX < (XMVectorGetX(textsize) + 2) * FontsList->GetFont("OpenSans_12")->GetScale().x) {
		textSizeX = (XMVectorGetX(textsize) + 2) * FontsList->GetFont("OpenSans_12")->GetScale().x;
	}
	text._Position = { _SizeOfScreen.x - textSizeX,textSizeY };
	textSizeY += XMVectorGetY(textsize) * FontsList->GetFont("OpenSans_12")->GetScale().y;
}
