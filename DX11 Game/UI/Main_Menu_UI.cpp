#include "stdafx.h"
#include "Main_Menu_UI.h"
#include <shellapi.h>
Main_Menu_UI::Main_Menu_UI()
{
	
}

Main_Menu_UI::~Main_Menu_UI()
{
	RemoveFromEvent();
}

void Main_Menu_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts)
{
	AddtoEvent();
	IsSettings = false;

	UI::Inizalize(device, contex, cb_vs_matrix_2d, fonts);


	FontsList->AddFont("OpenSans_12", "OpenSans_12.spritefont");
	Titlecard.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);


	MainMenuBackground.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 5; i++) {
		MainMenuButtions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

}

void Main_Menu_UI::Update(float dt)
{
	if (!IsSettings)
	{
		MainMenuBackground.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 0.7f);
		Titlecard.Function("Title_Card\\TitleCard.dds", { static_cast<float>(_SizeOfScreen.x * 0.4),static_cast<float>(_SizeOfScreen.y * 0.12) }, { static_cast<float>((_SizeOfScreen.x * 0.5) - static_cast<float>(_SizeOfScreen.x * 0.4) / 2) ,0 });

		MenuButtions();
		LinkButtions();

		
	}
}

void Main_Menu_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	Shaders::BindShaders(_Contex.Get(), vert, pix);
	MainMenuBackground.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
	Titlecard.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
	for (unsigned int i = 0; i < 5; i++) {
		MainMenuButtions[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get());
		Shaders::BindShaders(_Contex.Get(), vert, pix);
	}
}

void Main_Menu_UI::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::UpdateSettingsEvent:
	{
		IsSettings = false;
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

void Main_Menu_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
}

void Main_Menu_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateSettingsEvent, this);
}

void Main_Menu_UI::MenuButtions()
{
	XMFLOAT2 size{ static_cast<float>(_SizeOfScreen.x * 0.15), static_cast<float>(_SizeOfScreen.y * 0.13) };
	float ButtionXPos = static_cast<float>((_SizeOfScreen.x * 0.5) - size.x / 2);
	float ButtionYPos = 0.25f;
	if (MainMenuButtions[0].Function("Play", ButtionTex, size, XMFLOAT2{ ButtionXPos, static_cast<float>(_SizeOfScreen.y * ButtionYPos) }, DirectX::Colors::Black, _MouseData)) {
		//go to hub/save
		EventSystem::Instance()->AddEvent(EVENTID::HideCursorEvent);
		LevelTo = 0;
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &LevelTo);
	}
	ButtionYPos += 0.20;

	if (MainMenuButtions[1].Function("place holder", ButtionTex, size, XMFLOAT2{ ButtionXPos,  static_cast<float>(_SizeOfScreen.y * 0.40) }, DirectX::Colors::Black, _MouseData)) {
		//place holder
		LevelTo = 3;
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &LevelTo);
	}
	ButtionYPos += 0.20;
	if (MainMenuButtions[2].Function("Settings", ButtionTex, size, XMFLOAT2{ ButtionXPos,  static_cast<float>(_SizeOfScreen.y * 0.55) }, DirectX::Colors::Black, _MouseData)) {
		//settings
		IsSettings = true;
		EventSystem::Instance()->AddEvent(EVENTID::GameSettingsEvent);
	}
	ButtionYPos += 0.20;
	if (MainMenuButtions[3].Function("Exit", ButtionTex, size, XMFLOAT2{ ButtionXPos,  static_cast<float>(_SizeOfScreen.y * 0.70) }, DirectX::Colors::Black, _MouseData)) {
		//exit
		EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
	}
}

void Main_Menu_UI::LinkButtions()
{
	//link to git hub
	if (MainMenuButtions[4].Function("", ButtionTex2, { static_cast<float>(_SizeOfScreen.x * 0.055), static_cast<float>(_SizeOfScreen.y * 0.075) }, XMFLOAT2{ 2,  2 }, DirectX::Colors::Black, _MouseData)) {
		if (!openlink && open) {
			openlink = true;
		}

	}
	if (MainMenuButtions[4].GetIsPressed() == false) {
		open = true;
	}
	if (openlink) {
		ShellExecute(0, 0, L"https://github.com/kyle-robinson/directx-game", 0, 0, SW_SHOW);
		open = false;
		openlink = false;
	}
}
