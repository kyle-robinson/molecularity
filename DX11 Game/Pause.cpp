#include "stdafx.h"
#include "Pause.h"

Pause::Pause()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::GamePauseEvent, this);
}

Pause::~Pause()
{
}

void Pause::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{
	_isPuased = false;
	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;
	
	HeadderTextRenderer = make_shared<TextRenderer>("OpenSans_50.spritefont", _Device.Get(), _Contex.Get());
	PGTextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont", _Device.Get(), _Contex.Get());

	PuaseBakgtound.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 4; i++) {
		PuaseButtions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}
	
}

void Pause::Update()
{
	if (_isPuased) {
		//bakground
		PuaseBakgtound.Function({ 0,0,0 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 0.7f);

		//Buttions
		if (PuaseButtions[0].Function("Play", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0, 100 },DirectX::Colors::White, _MouseData)) {
			//back to game
			_isPuased = false;
		}
		else if (PuaseButtions[1].Function("Reset", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  200 }, DirectX::Colors::White, _MouseData)) {
			//reset level
		}
		else if (PuaseButtions[2].Function("Settings", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  300 }, DirectX::Colors::White, _MouseData)) {
			//settings
			EventSystem::Instance()->AddEvent(EVENTID::GAmeSettingsEvent);
		}
		else if (PuaseButtions[3].Function("Exit", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { 100, 50 }, XMFLOAT2{ 0,  400 }, DirectX::Colors::White, _MouseData)) {
			//exit
			EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
		}

		//text
		TextToDraw puaseText;
		puaseText._Colour = Colors::LightGreen;
		puaseText._Position = { 0,0 };
		puaseText._Text = "Pause";
		PuaseTextTitles.push_back(puaseText);

		puaseText._Colour = Colors::LightGreen;
		puaseText._Position = { _SizeOfScreen.x / 2,_SizeOfScreen.y / 3 };
		puaseText._Text = "Tip";
		PuaseTextTitles.push_back(puaseText);

		puaseText._Colour = Colors::LightGreen;
		puaseText._Position = { static_cast<float>(_SizeOfScreen.x * 0.5),static_cast<float>(_SizeOfScreen.y * 0.5) };
		puaseText._Text = "Tip Text";
		PuaseTextPG.push_back(puaseText);
	}
}

void Pause::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	Shaders::BindShaders(_Contex.Get(), vert, pix);
	if (_isPuased) {
		PuaseBakgtound.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		for (unsigned int i = 0; i < 4; i++) {
			PuaseButtions[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, PGTextRenderer.get());
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

void Pause::CleanUp()
{
 
}

//FONTS need to be passed in 
