#include "stdafx.h"
#include "Tutorial_UI.h"

Tutorial_UI::Tutorial_UI()
{
}

Tutorial_UI::~Tutorial_UI()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ToolModeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateSettingsEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameUnPauseEvent, this);
}

void Tutorial_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ToolModeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GameUnPauseEvent, this);
	//get key binds
	std::vector<JSON::SettingData> SettingsData = JSON::LoadSettings();

	for (auto& setting : SettingsData)
	{
		if (setting.Type == JSON::SettingType::ControllType)
		{

			string key = std::get<string>(setting.Setting);
			unsigned char* valChar = (unsigned char*)key.c_str();
			KeyBindes[setting.Name] = ConvertFromUnsignedCharTostring(*valChar);
			
		}
	}

	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;

	_TextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont", _Device.Get(), _Contex.Get());

	TextBackground.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 2; i++) {
		OutLines[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
		
	}

	CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
	_TextRenderer->UpdateViewPort(newViewport);



	CurrentState = TutorialState::GameTut;
	stateNo = 0;

	timer.Start();
}

void Tutorial_UI::Update(float dt)
{
	float yPos=0;
	float xpos = _SizeOfScreen.x - 500;
	TextBackground.Function({ 225,225,225 }, { 500,225 }, { _SizeOfScreen.x-500,0 }, 0.8f);
	
	TextToDraw text;
	text._Colour = DirectX::Colors::Black;
	//Game instruction
	if (CurrentState == TutorialState::GameTut)
	{
		//get to exsit
		text._Text = "The aim for you is to puzzle you way\nthrought the rooms that are given to you.";
		text._Position = { xpos,yPos };
		_TextList.push_back(text);
	}
	
	//tool instructions 
	else if (CurrentState == TutorialState::ToolTut)
	{

		//genral information
		text._Text = "The tool allows for you to change the propites of the cubes \nyou will find in the levels.";
		text._Position = { xpos,yPos };
		_TextList.push_back(text);

		//tool controlls 
		yPos += 50;
		text._Text = "Mouse Wheel scroll: Change Tool mode";
		text._Position = { xpos,yPos };
		_TextList.push_back(text);
		yPos += 20;
		text._Text = KeyBindes["Gun_State_One"]+":  Tool 1";
		text._Text +="\n"+ KeyBindes["Gun_State_Two"] + ":  Tool 2";
		text._Text += "\n" + KeyBindes["Gun_State_Three"] + ":  Tool 3";
		text._Text += "\n" + KeyBindes["Gun_State_Four"] + ":  Tool 4";
		text._Text += "\n" + KeyBindes["Gun_State_Five"] + ":  Tool 5";
		text._Text += "\n" + KeyBindes["Gun_State_Six"] + ":  Tool 6";

		text._Position = { xpos,yPos };
		_TextList.push_back(text);

	}
	//indvudual tool infromation
	else if (CurrentState == TutorialState::ToolTut2)
	{
		//tell user about current mode
		switch (Mode->GetTooltype())
		{
		case ToolType::Convert: {
			//tool 1

			text._Text = "Convert";
			text._Position = { xpos,yPos };
			_TextList.push_back(text);
			yPos += 20;
			text._Text = "This tool alows you to change the cubes material.";
			text._Position = { xpos,yPos };
			_TextList.push_back(text);

			yPos += 20;
			text._Position = { xpos,yPos };
			text._Text = "This Turns the object into ";
			switch (static_cast<int>(Mode->GetCurrentOption().boxtype))
			{
			case 0:
				text._Text += "mesh.";
				break;
			case 1:
				text._Text += "wood.";
				break;
			case 2:
				text._Text += "stone.";
				break;
			case 3:
				text._Text += "iron.";
				break;
			case 4:
				text._Text += "alien.";
				break;
			}
			_TextList.push_back(text);
		}
							  break;
		case ToolType::Resize: {
			//tool 2

			text._Text = "Resize:";
			text._Position = { xpos,yPos };
			_TextList.push_back(text);

			yPos += 20;
			text._Text = "This tool allows you to change the cubes size.";
			text._Position = { xpos,yPos };
			_TextList.push_back(text);

			yPos += 20;
			switch (static_cast<int>(Mode->GetCurrentOption().boxSize))
			{
			case 0:

				text._Text = "This shrinks the cube.";

				break;
			case 1:
				text._Text = "This resets the cube to normal size.";
				break;
			case 2:
				text._Text = "This largens the cube.";

				break;
			}
			text._Position = { xpos,yPos };
			_TextList.push_back(text);
		}
							 break;

		}
	}
	//movement instructions
	else if(CurrentState==TutorialState::MovementTut)
	{
		//movement
		text._Text = "Movement Controls:";
		text._Text += "\n"+ KeyBindes["Forward"]+": Forward ";
		text._Text += "\n" + KeyBindes["Left"] + ": Left";
		text._Text += "\n" + KeyBindes["Back"] + ": Back";
		text._Text += "\n" + KeyBindes["Right"] + ": Right";
		text._Text += "\n" + KeyBindes["Jump"] + ": Jump";
		text._Text += "\nMouse: Look around";
		text._Position = { xpos,yPos };
		_TextList.push_back(text);
	}

	//other controlls 
	else if (CurrentState == TutorialState::OtherTut)
	{
		
		text._Text = "Other Controls";
		text._Text += "\n" + KeyBindes["Pause"] + ": Puase";
		text._Text += "\n" + KeyBindes["Change_Gun_State_Up"] + ": Change tool up";
		text._Text += "\n" + KeyBindes["Change_Gun_State_Down"] + ": Change tool up";
		text._Position = { xpos,yPos };
		_TextList.push_back(text);
	}


	//continue 
	{
		//wait time 

		if (timer.GetMilliSecondsElapsed()>= 5000 && stateNo<5) {


			text._Text = "press enter to continue";
			text._Position = { _SizeOfScreen.x - 500,200 };
			_TextList.push_back(text);
			//get is enter key down
			if ((1 << 15) & GetAsyncKeyState(VK_RETURN) ) {
				//move next state
				stateNo++;
				CurrentState = static_cast<TutorialState>(stateNo);
				timer.Restart();
			}
			
		}

	}
	if (stateNo > 4) {
		//end
		EventSystem::Instance()->AddEvent(EVENTID::UITutorialEnd);
		EventSystem::Instance()->AddEvent(EVENTID::RemoveUIItemEvent,&name);
		timer.Stop();
	}


}

void Tutorial_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	//isDraw to stop from drawing over Puase/settings
	if (IsDraw) {
		Shaders::BindShaders(_Contex.Get(), vert, pix);
		TextBackground.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);

		for (auto& Text : _TextList)
		{
			_TextRenderer->RenderString(Text._Text, Text._Position, Text._Colour);
		}

		_TextList.clear();

	}
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
		std::vector<JSON::SettingData> SettingsData = JSON::LoadSettings();

		for (auto& setting : SettingsData)
		{
			if (setting.Type == JSON::SettingType::ControllType)
			{

				string key = std::get<string>(setting.Setting);
				unsigned char* valChar = (unsigned char*)key.c_str();
				KeyBindes[setting.Name] = ConvertFromUnsignedCharTostring(*valChar);

			}
		}
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
		CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
		_TextRenderer->UpdateViewPort(newViewport);
	}
	break;
	}
	
}

//convert from unsigned char to understandble string fromat 
string Tutorial_UI::ConvertFromUnsignedCharTostring(unsigned char input)
{

	string text;
	switch (input)
	{
		//other keys
	case VK_RETURN:
		text = "Return";
		
		break;
	case VK_SPACE:
		text = "Space";
	
		break;
	case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
		text = "Shift";
		
		break;
	case VK_CONTROL:
		text = "Ctrl";
		
		break;
		//alt
	case VK_MENU:
		text = "alt";
		
		break;
	case VK_PAUSE:
		text = "Pause";
	
		break;
	case VK_ESCAPE:
		text = "ESC";
		
		break;
	case VK_CAPITAL:
		text = "CAPS LOCK";
		
		break;
	case VK_BACK:
		text = "BACKSPACE";
		
		break;
	case VK_TAB:
		text = "Tab";
		
		break;
		//page up
	case VK_PRIOR:
		text = "page Up";
		
		break;
		//page down
	case VK_NEXT:
		text = "page Down";
		
		break;
	case VK_END:
		text = "End";
		
	case VK_HOME:
		text = "Home";
		
		break;
	case VK_SELECT:
		text = "Select";
		
		break;
	case VK_INSERT:
		text = "Ins";
	
		break;
	case VK_DELETE:
		text = "Del";
		
		break;
	case VK_HELP:
		text = "Help";
		
		break;
	case VK_SCROLL:
		text = "Scr Lock";
		
		break;
	case VK_CLEAR:
		text = "Clear";
		
		break;
		//arrow keys
	case VK_UP:
		text = "Up Arrow";
		
		break;
	case VK_DOWN:
		text = "Down Arrow";
		
		break;
	case VK_LEFT:
		text = "Left Arrow";
		
		break;
	case VK_RIGHT:
		text = "Right Arrow";
		
		break;


		//F keys
	case VK_F1:
		text = "F1";
		
		break;
	case VK_F2:
		text = "F2";
	
		break;
	case VK_F3:
		text = "F3";
		
		break;
	case VK_F4:
		text = "F4";
		
		break;
	case VK_F5:
		text = "F5";
		
		break;
	case VK_F6:
		text = "F6";
		
		break;
	case VK_F7:
		text = "F7";
		
		break;
	case VK_F8:
		text = "F8";
		
		break;
	case VK_F9:
		text = "F9";
		
		break;
	case VK_F10:
		text = "F10";
		
		break;
	case VK_F11:
		text = "F11";
		
		break;
	case VK_F12:
		text = "F12";
	
		break;

		//numpad
	case VK_NUMLOCK:
		text = "Numlock";
		
		break;
	case VK_NUMPAD0:
		text = "Numpad 0";
		
		break;
	case VK_NUMPAD1:
		text = "Numpad 1";
		
		break;
	case VK_NUMPAD2:
		text = "Numpad 2";
		
		break;
	case VK_NUMPAD3:
		text = "Numpad 3";
		
		break;
	case VK_NUMPAD4:
		text = "Numpad 4";
		
		break;
	case VK_NUMPAD5:
		text = "Numpad 5";
		
		break;
	case VK_NUMPAD6:
		text = "Numpad 6";
		
		break;
	case VK_NUMPAD7:
		text = "Numpad 7";
		
		break;
	case VK_NUMPAD8:
		text = "Numpad 8";
		
		break;
	case VK_NUMPAD9:
		text = "Numpad 9";
		
		break;


		//math keys
	case VK_MULTIPLY:
		text = "*";
		
		break;
	case VK_ADD:
		text = "+";
		
		break;
	case VK_SEPARATOR:
		text = "";
		
		break;
	case VK_SUBTRACT:
		text = "-";
		
		break;
	case VK_DECIMAL:
		text = ".";
		
		break;
	case VK_DIVIDE:
		text = "/";
	
		break;

		//number key codes
		//Braket left
	case 219:
		text = "[";
		
		break;
		//Braket right
	case 221:
		text = "]";
		
		break;

	case 222:
		text = "#";
		
		break;
	case 186:
		text = ";";
		
		break;
	case 192:
		text = "'";
		
	case 188:
		text = ",";
		
		break;
	case 187:
		text = "=";
	
		break;
	case 223:
		text = "`";
		
		break;
	case 220:
		text = "\\";
		
		break;
	case 191:
		text = "/";
		
		break;
	case 190:
		text = ".";
	
		break;

		//all other keys
	default:
		
			text = input;
			
		
		break;
	}

	return text;
}
