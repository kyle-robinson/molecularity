#include "stdafx.h"
#include "Credits_UI.h"

Credits_UI::Credits_UI()
{
}

Credits_UI::~Credits_UI()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
}

void Credits_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{

	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	
	_Device = device;
	_Contex = contex;
	_cb_vs_matrix_2d = cb_vs_matrix_2d;

	_TextRenderer = make_shared<TextRenderer>("OpenSans_20.spritefont", _Device.Get(), _Contex.Get());
	
	
	Background.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);

	
	CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, _SizeOfScreen.x, _SizeOfScreen.y);
	_TextRenderer->UpdateViewPort(newViewport);
	
	

	//credits data
	//TODO Load fromn json
	//genral
	CreditsINFO.push_back(make_pair("", "Engine"));
	CreditsINFO.push_back(make_pair("Lead programmer", "Kyle ROBINSON "));
	CreditsINFO.push_back(make_pair("Engine programmer", "Daniel WILSON"));
	CreditsINFO.push_back(make_pair("Engine programmer", "Ben SHAW"));
	CreditsINFO.push_back(make_pair("Engine programmer", "Jacob J DEXTER"));
	CreditsINFO.push_back(make_pair("Engine programmer", "Thomas MILLARD"));

	//sound
	CreditsINFO.push_back(make_pair("", "Sound"));
	CreditsINFO.push_back(make_pair("Sound", "Ben SHAW"));
	
	//game desing
	CreditsINFO.push_back(make_pair("", "Game design"));
	CreditsINFO.push_back(make_pair("Game design", "Ben SHAW"));
	CreditsINFO.push_back(make_pair("Game design", "Kyle ROBINSON"));
	CreditsINFO.push_back(make_pair("Game design", "Daniel WILSON"));
	CreditsINFO.push_back(make_pair("Game design", "Jacob J DEXTER"));

	//graphics
	CreditsINFO.push_back(make_pair("", "Graphics"));
	CreditsINFO.push_back(make_pair("Graphics Programmer", "Kyle ROBINSON "));
	//UI
	CreditsINFO.push_back(make_pair("", "UI"));
	CreditsINFO.push_back(make_pair("UI programmer", "Thomas MILLARD"));
	CreditsINFO.push_back(make_pair("UI art", "Thomas MILLARD"));
	CreditsINFO.push_back(make_pair("UI desigp", "Thomas MILLARD"));
	
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


	//immage data
	ImmageData IData;

	IData.Name = "Headder";
	IData.FileName = "Title_Card\\TitleCard.dds";
	IData.Size = { static_cast<float>(_SizeOfScreen.x * 0.5),static_cast<float>(_SizeOfScreen.y * 0.12) };
	_ImmageList.push_back(IData);


	IData.Name = "DissCube";
	IData.FileName = "Credits\\Disscube.png";
	IData.Size = { static_cast<float>(_SizeOfScreen.x * 0.20),static_cast<float>(_SizeOfScreen.y * 0.24) };
	_ImmageList.push_back(IData);


	IData.Name = "Dimond";
	IData.FileName = "Settings\\Slider_Yellow.dds";
	IData.Size = { static_cast<float>(_SizeOfScreen.x * 0.05),static_cast<float>(_SizeOfScreen.y * 0.05) };
	_ImmageList.push_back(IData);

	
	for (unsigned int i = 0; i < _ImmageList.size(); i++) {
		Images[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

	//start pos
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
		XMVECTOR textsize = { 0,0,0 };
		if (Text.first != "") {
			textsize = _TextRenderer->GetSpriteFont()->MeasureString(Text.first.c_str());
			TextForDraw._Position = { ((_SizeOfScreen.x / 2) - (XMVectorGetX(textsize) * _TextRenderer->GetScale().x))-50,YPos };
			TextForDraw._Text = Text.first;
			_TextList.push_back(TextForDraw);

			TextForDraw._Position = { (_SizeOfScreen.x / 2)+50 ,YPos };
			TextForDraw._Text = Text.second;
			_TextList.push_back(TextForDraw);
		}
		else
		{
			
		
			if (imagecount < _ImmageList.size()) {
					//insert immage
					Images[imagecount].Function(_ImmageList[imagecount].FileName, _ImmageList[imagecount].Size, { (_SizeOfScreen.x / 2) - (_ImmageList[imagecount].Size.x / 2),YPos });
					YPos += _ImmageList[imagecount].Size.y + 10;
					imagecount++;
					
				}
			 textsize = _TextRenderer->GetSpriteFont()->MeasureString(Text.second.c_str());
			TextForDraw._Position = { (_SizeOfScreen.x / 2) - ((XMVectorGetX(textsize)/2)* _TextRenderer->GetScale().x),YPos };
			TextForDraw._Text = Text.second;
			
			_TextList.push_back(TextForDraw);
		}
		
		YPos += (XMVectorGetY(textsize) * _TextRenderer->GetScale().y);
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


	for (UINT i = 0; i < imagecount; i++)
	{
		Images[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);

	}

	imagecount = 0;
	
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
