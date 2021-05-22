#include "stdafx.h"
#include "Credits_UI.h"

Credits_UI::Credits_UI()
{
}

Credits_UI::~Credits_UI()
{
	RemoveFromEvent();
}

void Credits_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts)
{

	AddtoEvent();
	UI::Inizalize(device, contex, cb_vs_matrix_2d, fonts);

	FontsList->AddFont("OpenSans_20", "OpenSans_20.spritefont");

	Background.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);

	LoadText();


	LoadImages();
	//start pos
	NextY = _SizeOfScreen.y;
}


void Credits_UI::Update(float dt)
{
	//background 
	Background.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 1.0f);
	
	
	CurrentYPos = NextY;
	NextY -= 0.1f*dt;
	AddText();
	
	


	//check it is at end
	if (CurrentYPos < 0) {
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
		FontsList->GetFont("OpenSans_20")->RenderString(Text._Text, Text._Position, Text._Colour);
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
		
	}
	break;
	}
}

void Credits_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
}

void Credits_UI::RemoveFromEvent()
{
	RemoveFromEvent();
}

void Credits_UI::LoadText()
{
	//credits data

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
}

void Credits_UI::LoadImages()
{
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

}

void Credits_UI::AddImmage()
{
	if (imagecount < _ImmageList.size()) {
		//insert immage
		Images[imagecount].Function(_ImmageList[imagecount].FileName, _ImmageList[imagecount].Size, { (_SizeOfScreen.x / 2) - (_ImmageList[imagecount].Size.x / 2),CurrentYPos });
		CurrentYPos += _ImmageList[imagecount].Size.y + 10;
		imagecount++;

	}
}

void Credits_UI::AddText()
{
	//names
	TextToDraw TextForDraw;
	TextForDraw._Colour = DirectX::Colors::Black;
	for (auto& Text : CreditsINFO)
	{
		XMVECTOR textsize = { 0,0,0 };
		if (Text.first != "") {
			textsize = FontsList->GetFont("OpenSans_20")->GetSpriteFont()->MeasureString(Text.first.c_str());
			TextForDraw._Position = { ((_SizeOfScreen.x / 2) - (XMVectorGetX(textsize) * FontsList->GetFont("OpenSans_20")->GetScale().x)) - 50,CurrentYPos };
			TextForDraw._Text = Text.first;
			_TextList.push_back(TextForDraw);

			TextForDraw._Position = { (_SizeOfScreen.x / 2) + 50 ,CurrentYPos };
			TextForDraw._Text = Text.second;
			_TextList.push_back(TextForDraw);
		}
		else
		{


			AddImmage();
			textsize = FontsList->GetFont("OpenSans_20")->GetSpriteFont()->MeasureString(Text.second.c_str());
			TextForDraw._Position = { (_SizeOfScreen.x / 2) - ((XMVectorGetX(textsize) / 2) * FontsList->GetFont("OpenSans_20")->GetScale().x),CurrentYPos };
			TextForDraw._Text = Text.second;

			_TextList.push_back(TextForDraw);
		}

		CurrentYPos += (XMVectorGetY(textsize) * FontsList->GetFont("OpenSans_20")->GetScale().y);
	}

}
