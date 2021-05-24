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
	_isPuased = false;

	
	FontsList->AddFont("OpenSans_50", "OpenSans_50.spritefont");
	FontsList->AddFont("OpenSans_12", "OpenSans_12.spritefont");


	_TitleCard.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	PuaseBakgtound.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 4; i++) {
		PuaseButtions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}


}

void Pause::Update(float dt)
{
	if (_isPuased) {
		//bakground
		PuaseBakgtound.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 0.7f);
		//title card
		_TitleCard.Function("Title_Card\\TitleCard.dds", { static_cast<float>(_SizeOfScreen.x*0.5),static_cast<float>(_SizeOfScreen.y*0.12) }, { 0,0 });
		ButtionCreate();

		//Headder text
		TextToDraw puaseText;
		puaseText._Colour = Colors::Black;
		puaseText._Position = { 0,static_cast<float>(_SizeOfScreen.y * 0.12) };
		puaseText._Text = "Pause";
		PuaseTextTitles.push_back(puaseText);

		AddTipText();
	}
}

void Pause::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	Shaders::BindShaders(_Contex.Get(), vert, pix);
	if (_isPuased) {
		PuaseBakgtound.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		_TitleCard.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		for (unsigned int i = 0; i < 4; i++) {
			PuaseButtions[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get());
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
	}
}

void Pause::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::GamePauseEvent:
	{
		_isPuased = true;
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

void Pause::ButtionCreate()
{
	//Buttions
	if (PuaseButtions[0].Function("Play", ButtionTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0, static_cast<float>(_SizeOfScreen.y * 0.25) }, DirectX::Colors::Black, _MouseData)) {
		//back to game
		_isPuased = false;
		EventSystem::Instance()->AddEvent(EVENTID::HideCursorEvent);

	}
	else if (PuaseButtions[1].Function("Main Menu", ButtionTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.40) }, DirectX::Colors::Black, _MouseData)) {
		//reset level
		_isPuased = false;
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &Hub);
	}
	else if (PuaseButtions[2].Function("Settings", ButtionTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.55) }, DirectX::Colors::Black, _MouseData)) {
		//settings
		EventSystem::Instance()->AddEvent(EVENTID::GameSettingsEvent);
	}
	else if (PuaseButtions[3].Function("Exit", ButtionTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.70) }, DirectX::Colors::Black, _MouseData)) {
		//exit
		EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
	}
}

void Pause::AddTipText()
{
	TextToDraw puaseText;
	XMVECTOR FontSize;
	puaseText._Colour = Colors::Black;

	puaseText._Text = "Tip";
	FontSize=FontsList->GetFont("OpenSans_50")->GetSpriteFont()->MeasureString(puaseText._Text.c_str());
	puaseText._Position = { (_SizeOfScreen.x / 2)-(XMVectorGetX(FontSize)/2),_SizeOfScreen.y / 3 };
	PuaseTextTitles.push_back(puaseText);

	switch (currentLevel)
	{
	case 0:
		puaseText._Text = "Use the Buttion on the other side";
		break;
	case 1:
		puaseText._Text = "Use the tool";
		break;
	default:
		puaseText._Text = "This Is TEXT";
		break;
	}
	
	FontSize = FontsList->GetFont("OpenSans_12")->GetSpriteFont()->MeasureString(puaseText._Text.c_str());
	puaseText._Position = { static_cast<float>(_SizeOfScreen.x * 0.5) - (XMVectorGetX(FontSize) / 2),static_cast<float>(_SizeOfScreen.y * 0.5) };
	PuaseTextPG.push_back(puaseText);
}
