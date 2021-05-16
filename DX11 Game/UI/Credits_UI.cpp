#include "stdafx.h"
#include "Credits_UI.h"

Credits_UI::Credits_UI()
{
}

Credits_UI::~Credits_UI()
{
}

void Credits_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{

	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	
	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;

	_TextRenderer = make_shared<TextRenderer>("OpenSans_50.spritefont", _Device.Get(), _Contex.Get());
	
	
	Background.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	for (unsigned int i = 0; i < 2; i++) {
		Images[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

	CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
	_TextRenderer->UpdateViewPort(newViewport);
	
	

	//credits data
	
	//genral
	CreditsINFO.push_back(make_pair("Lead programmer", "Kyle ROBINSON "));
	CreditsINFO.push_back(make_pair("engine programmer", "Daniel WILSON"));
	CreditsINFO.push_back(make_pair("engine programmer", "Ben SHAW"));
	CreditsINFO.push_back(make_pair("engine programmer", "Jacob J DEXTER"));
	CreditsINFO.push_back(make_pair("engine programmer", "Thomas MILLARD"));

	//sound
	CreditsINFO.push_back(make_pair("", "Sound"));
	CreditsINFO.push_back(make_pair("Sound", "Ben SHAW"));
	
	//game desing
	CreditsINFO.push_back(make_pair("", "Game Desing"));
	CreditsINFO.push_back(make_pair("Game Desing", "Ben SHAW"));
	CreditsINFO.push_back(make_pair("Game Desing", "Kyle ROBINSON"));
	CreditsINFO.push_back(make_pair("Game Desing", "Daniel WILSON"));
	CreditsINFO.push_back(make_pair("Game Desing", "Jacob J DEXTER"));

	//UI
	CreditsINFO.push_back(make_pair("", "UI"));
	CreditsINFO.push_back(make_pair("UI programming", "Thomas MILLARD"));
	CreditsINFO.push_back(make_pair("UI art", "Thomas MILLARD"));
	CreditsINFO.push_back(make_pair("UI desing", "Thomas MILLARD"));
	
	CreditsINFO.push_back(make_pair("", "Physics"));
	CreditsINFO.push_back(make_pair("Physics", "Jacob J DEXTER"));
	
	//art/3d models
	CreditsINFO.push_back(make_pair("", "3D Models"));
	CreditsINFO.push_back(make_pair("3D Models", "Daniel WILSON"));
	
	CreditsINFO.push_back(make_pair("", "Other"));
	CreditsINFO.push_back(make_pair("???", "Lorcan B ENGLEDOW"));
	
	//end
	CreditsINFO.push_back(make_pair("Thank", "You"));
	CreditsINFO.push_back(make_pair("For", "Playing"));

	minus = _SizeOfScreen.y;
}

void Credits_UI::Update(float dt)
{
	//background 
	Background.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 1.0f);
	
	YPos = minus;

	//title card 
	Images[0].Function("Title_Card\\TitleCard.dds", { static_cast<float>(_SizeOfScreen.x * 0.5),static_cast<float>(_SizeOfScreen.y * 0.12) }, { (_SizeOfScreen.x / 2)- (static_cast<float>(_SizeOfScreen.x * 0.5)/2),YPos });
	YPos += 100;
	//names
	TextToDraw TextForDraw;
	TextForDraw._Colour = DirectX::Colors::Black;
	for (auto &Text: CreditsINFO )
	{	
		
		if (Text.first != "") {
			XMVECTOR textsize = _TextRenderer->GetSpriteFont()->MeasureString(Text.first.c_str());
			TextForDraw._Position = { ((_SizeOfScreen.x / 2) - XMVectorGetX(textsize))-50,YPos };
			TextForDraw._Text = Text.first;
			_TextList.push_back(TextForDraw);

			TextForDraw._Position = { (_SizeOfScreen.x / 2)+50 ,YPos };
			TextForDraw._Text = Text.second;
			_TextList.push_back(TextForDraw);
		}
		else
		{
			XMVECTOR textsize = _TextRenderer->GetSpriteFont()->MeasureString(Text.second.c_str());
			TextForDraw._Position = { (_SizeOfScreen.x / 2) - (XMVectorGetX(textsize)/2) ,YPos };
			TextForDraw._Text = Text.second;
			_TextList.push_back(TextForDraw);
		}
		
		YPos += 100;
	}

	
	minus -= 1;


	//check it is at end
	if (YPos < 0) {
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &LevelTo);
	}

}

void Credits_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	Background.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
	Images[0].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);

	for (auto& Text : _TextList)
	{
		_TextRenderer->RenderString(Text._Text, Text._Position, Text._Colour);
	}

	_TextList.clear();

}

void Credits_UI::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	
	case EVENTID::WindowSizeChangeEvent:
	{
		_SizeOfScreen = *static_cast<XMFLOAT2*>(event->GetData());
		CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
		_TextRenderer->UpdateViewPort(newViewport);
	}
	break;
	}
}
