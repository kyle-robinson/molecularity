#include "stdafx.h"
#include "Pause.h"

Pause::Pause()
{
	
}

Pause::~Pause()
{
	RemoveFromEvent();
}

void Pause::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts)
{

	AddtoEvent();
	UI::Inizalize(device, contex, cb_vs_matrix_2d,fonts);
	_isPaused = false;

	//text
	TextLoad();

	FontsList->AddFont("OpenSans_50", "OpenSans_50.spritefont");
	FontsList->AddFont("OpenSans_12", "OpenSans_12.spritefont");


	_TitleCard.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	PauseBackground.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 4; i++) {
		PauseButtons[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}


}

void Pause::Update(float dt)
{
	if (_isPaused) {
		//bakground
		PauseBackground.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 0.7f);
		//title card
		_TitleCard.Function("Title_Card\\TitleCard.png", { static_cast<float>(_SizeOfScreen.x*0.5),static_cast<float>(_SizeOfScreen.y*0.12) }, { 0,0 });
		ButtonCreate();

		//Headder text
		TextToDraw pauseText;
		pauseText._Colour = Colors::Black;
		pauseText._Position = { 0,static_cast<float>(_SizeOfScreen.y * 0.12) };
		pauseText._Text = LoadedTextMap["Title"];
		PauseTextTitles.push_back(pauseText);

		AddTipText();
	}
}

void Pause::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	Shaders::BindShaders(_Contex.Get(), vert, pix);
	if (_isPaused) {
		PauseBackground.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		_TitleCard.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		for (unsigned int i = 0; i < 4; i++) {
			PauseButtons[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (UINT i = 0; i < PauseTextTitles.size(); i++)
		{
			FontsList->GetFont("OpenSans_50")->RenderString(PauseTextTitles[i]._Text, PauseTextTitles[i]._Position, PauseTextTitles[i]._Colour);
		}
		PauseTextTitles.clear();
		for (UINT i = 0; i < PauseTextPG.size(); i++)
		{
			FontsList->GetFont("OpenSans_12")->RenderString(PauseTextPG[i]._Text, PauseTextPG[i]._Position, PauseTextPG[i]._Colour);
		}
		PauseTextPG.clear();
	}
}

void Pause::TextLoad()
{
	vector<JSON::TextData>ButtonsText = TextLoader::Instance()->LoadText("Pause_Buttons");
	LoadedTextMap = TextLoader::Instance()->ConvertToMap(ButtonsText);
	vector<JSON::TextData>TipText = TextLoader::Instance()->LoadText("Tip_Pause");
	map<string, string>temp= TextLoader::Instance()->ConvertToMap(TipText);
	LoadedTextMap.insert(temp.begin(), temp.end());
	vector<JSON::TextData>TitleText = TextLoader::Instance()->LoadText("Pause_Text");
	temp = TextLoader::Instance()->ConvertToMap(TitleText);
	LoadedTextMap.insert(temp.begin(), temp.end());
	
}

void Pause::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::GamePauseEvent:
	{
		_isPaused = true;
	}
	break;
	case EVENTID::SetCurrentLevelEvent:
	{
		currentLevel = *static_cast<int*>(event->GetData());
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
	
		_MouseData.LPress = false;
	}
	break;
	}
}

void Pause::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::SetCurrentLevelEvent, this);
}

void Pause::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::SetCurrentLevelEvent, this);
}

void Pause::ButtonCreate()
{
	//Buttons
	if (PauseButtons[0].Function(LoadedTextMap["Button_1"], ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0, static_cast<float>(_SizeOfScreen.y * 0.25) }, DirectX::Colors::Black, _MouseData)) {
		//back to game
		_isPaused = false;
		EventSystem::Instance()->AddEvent(EVENTID::GameUnPauseEvent);
		EventSystem::Instance()->AddEvent(EVENTID::HideCursorEvent);
		
	}
	else if (PauseButtons[1].Function(LoadedTextMap["Button_2"], ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.40) }, DirectX::Colors::Black, _MouseData)) {
		//reset level
		_isPaused = false;
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &Hub);
	}
	else if (PauseButtons[2].Function(LoadedTextMap["Button_3"], ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.55) }, DirectX::Colors::Black, _MouseData)) {
		//settings
		EventSystem::Instance()->AddEvent(EVENTID::GameSettingsEvent);
	}
	else if (PauseButtons[3].Function(LoadedTextMap["Button_4"], ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.70) }, DirectX::Colors::Black, _MouseData)) {
		//exit
		EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
	}
}

void Pause::AddTipText()
{
	TextToDraw pauseText;
	XMVECTOR FontSize;
	pauseText._Colour = Colors::Black;

	pauseText._Text = LoadedTextMap["Tip_Title"];
	FontSize=FontsList->GetFont("OpenSans_50")->GetSpriteFont()->MeasureString(pauseText._Text.c_str());
	pauseText._Position = { (_SizeOfScreen.x / 2)-(XMVectorGetX(FontSize)/2),_SizeOfScreen.y / 3 };
	PauseTextTitles.push_back(pauseText);

	switch (currentLevel)
	{
	case 1:
		pauseText._Text = LoadedTextMap["Tip_Level_1"];
		break;
	case 2:
		pauseText._Text = LoadedTextMap["Tip_Level_2"];
		break;
	case 3:
		pauseText._Text = LoadedTextMap["Tip_Level_3"];
		break;

	case 4:
		pauseText._Text = LoadedTextMap["Tip_Level_4"];
		break;
	default:
		pauseText._Text = "This Is TEXT";
		break;
	}
	
	FontSize = FontsList->GetFont("OpenSans_12")->GetSpriteFont()->MeasureString(pauseText._Text.c_str());
	pauseText._Position = { static_cast<float>(_SizeOfScreen.x * 0.5) - (XMVectorGetX(FontSize) / 2),static_cast<float>(_SizeOfScreen.y * 0.5) };
	PauseTextPG.push_back(pauseText);
}
