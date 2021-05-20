#include "stdafx.h"
#include "EndLevelScreen_UI.h"

EndLevelScreen_UI::EndLevelScreen_UI()
{
}

EndLevelScreen_UI::~EndLevelScreen_UI()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::SetNextLevelEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameEndLevelEvent, this);
}

void EndLevelScreen_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::SetNextLevelEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GameEndLevelEvent, this);
	
	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;

	_TextRenderer50 = make_shared<TextRenderer>("OpenSans_50.spritefont", _Device.Get(), _Contex.Get());
	_TextRenderer12 = make_shared<TextRenderer>("OpenSans_12.spritefont", _Device.Get(), _Contex.Get());
	CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
	_TextRenderer50->UpdateViewPort(newViewport);
	_TextRenderer12->UpdateViewPort(newViewport);
	Background.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 2; i++) {
		Images[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}
	for (unsigned int i = 0; i < 2; i++) {
		Buttions[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

}

void EndLevelScreen_UI::Update(float dt)
{
	if (ToShow) {
		//background
		Background.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 1.0f);
		//title card
		Images[0].Function("Title_Card\\TitleCard.dds", { static_cast<float>(_SizeOfScreen.x * 0.5),static_cast<float>(_SizeOfScreen.y * 0.12) }, { (_SizeOfScreen.x / 2) - (static_cast<float>(_SizeOfScreen.x * 0.5) / 2),0 });
		//buttions
		if (Buttions[0].Function("Main Menu", ButtionTex, { _SizeOfScreen.x / 7, _SizeOfScreen.y / 7 }, XMFLOAT2{ (_SizeOfScreen.x/2)- (_SizeOfScreen.x / 7)-50, static_cast<float>(_SizeOfScreen.y * 0.25) }, DirectX::Colors::Black, _MouseData)) {
			//go to hub
			EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &Hub);
			ToShow = false;
		}
		else if (Buttions[1].Function("Next Level", ButtionTex, { _SizeOfScreen.x / 7, _SizeOfScreen.y / 7 }, XMFLOAT2{ (_SizeOfScreen.x / 2) +50, static_cast<float>(_SizeOfScreen.y * 0.25) }, DirectX::Colors::Black, _MouseData)) {
			//go to next level
			EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &NextLevel);
			ToShow = false;
		}

		TextToDraw text;
		text._Text = "Level Complet";

		XMVECTOR textsize= _TextRenderer50->GetSpriteFont()->MeasureString(text._Text.c_str());
		text._Position = { (_SizeOfScreen.x / 2) - (XMVectorGetX(textsize)/2),static_cast<float>(_SizeOfScreen.y * 0.12) };
		text._Colour = DirectX::Colors::Black;
		_TextList.push_back(text);
	}
}

void EndLevelScreen_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{

	Shaders::BindShaders(_Contex.Get(), vert, pix);
	if (ToShow) {
		Background.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		Images[0].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		for (unsigned int i = 0; i < 2; i++) {
			Buttions[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, _TextRenderer12.get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (auto& Text : _TextList)
		{
			_TextRenderer50->RenderString(Text._Text, Text._Position, Text._Colour);
		}
		_TextList.clear();
	}


}

void EndLevelScreen_UI::HandleEvent(Event* event)
{

	switch (event->GetEventID())
	{
		case EVENTID::GameEndLevelEvent:
		{
			ToShow = true;
			EventSystem::Instance()->AddEvent(EVENTID::GamePauseEvent);
		}
		break;
		case EVENTID::SetNextLevelEvent:
		{
			NextLevel = *static_cast<int*>(event->GetData());
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
			CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
			_TextRenderer50->UpdateViewPort(newViewport);
			_TextRenderer12->UpdateViewPort(newViewport);
			_MouseData.LPress = false;
		}
	}




}
