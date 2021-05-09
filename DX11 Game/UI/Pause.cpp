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
	_isPaused = false;
	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;
	
	HeadderTextRenderer = make_shared<TextRenderer>("OpenSans_50.spritefont", _Device.Get(), _Contex.Get());
	PGTextRenderer = make_shared<TextRenderer>("OpenSans_12.spritefont", _Device.Get(), _Contex.Get());
	_TitleCard.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	PauseBackground.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (uint32_t i = 0; i < 4; i++) {
		PauseButtons[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}
	
}

void Pause::Update()
{
	if (_isPaused) {
		//bakground
		PauseBackground.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 0.7f);
		_TitleCard.Function("Title_Card\\TitleCard.dds", { static_cast<float>(_SizeOfScreen.x*0.5),static_cast<float>(_SizeOfScreen.y*0.12) }, { 0,0 });
		//Buttons
		if (PauseButtons[0].Function("Play", ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0, static_cast<float>( _SizeOfScreen.y*0.25) },DirectX::Colors::Black, _MouseData)) {
			//back to game
			_isPaused = false;
		}
		else if (PauseButtons[1].Function("Reset", ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.40) }, DirectX::Colors::Black, _MouseData)) {
			//reset level
		}
		else if (PauseButtons[2].Function("Settings", ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.55) }, DirectX::Colors::Black, _MouseData)) {
			//settings
			EventSystem::Instance()->AddEvent(EVENTID::GAmeSettingsEvent);
		}
		else if (PauseButtons[3].Function("Exit", ButtonTex, { _SizeOfScreen.x / 10, _SizeOfScreen.y / 10 }, XMFLOAT2{ 0,  static_cast<float>(_SizeOfScreen.y * 0.70) }, DirectX::Colors::Black, _MouseData)) {
			//exit
			EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
		}

		//text
		TextToDraw puaseText;
		puaseText._Colour = Colors::Black;
		puaseText._Position = { 0,static_cast<float>(_SizeOfScreen.y * 0.12) };
		puaseText._Text = "Pause";
		PauseTextTitles.push_back(puaseText);

		puaseText._Colour = Colors::Black;
		puaseText._Position = { _SizeOfScreen.x / 2,_SizeOfScreen.y / 3 };
		puaseText._Text = "Tip";
		PauseTextTitles.push_back(puaseText);

		puaseText._Colour = Colors::Black;
		puaseText._Position = { static_cast<float>(_SizeOfScreen.x * 0.5),static_cast<float>(_SizeOfScreen.y * 0.5) };
		puaseText._Text = "Tip Text";
		PauseTextPG.push_back(puaseText);
	}
}

void Pause::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	Shaders::BindShaders(_Contex.Get(), vert, pix);
	if (_isPaused) {
		PauseBackground.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		_TitleCard.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		for (uint32_t i = 0; i < 4; i++) {
			PauseButtons[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, PGTextRenderer.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (UINT i = 0; i < PauseTextTitles.size(); i++)
		{
			HeadderTextRenderer->RenderString(PauseTextTitles[i]._Text, PauseTextTitles[i]._Position, PauseTextTitles[i]._Colour);
		}
		PauseTextTitles.clear();
		for (UINT i = 0; i < PauseTextPG.size(); i++)
		{
			PGTextRenderer->RenderString(PauseTextPG[i]._Text, PauseTextPG[i]._Position, PauseTextPG[i]._Colour);
		}
		PauseTextPG.clear();
	}
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
		

		D3D11_VIEWPORT a;
		a.TopLeftX = 0;
		a.TopLeftY = 0;
		a.Width = _SizeOfScreen.x;
		a.Height = _SizeOfScreen.y;
		a.MaxDepth = 1.0f;
		a.MinDepth = 0.0f;

		HeadderTextRenderer->UpdateViewPort(a);
		PGTextRenderer->UpdateViewPort(a);
	}
	break;

	}
}

void Pause::CleanUp()
{
 
}

//FONTS need to be passed in 
