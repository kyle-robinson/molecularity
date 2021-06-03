#include "stdafx.h"
#include "EndLevelScreen_UI.h"

EndLevelScreen_UI::EndLevelScreen_UI()
{
}

EndLevelScreen_UI::~EndLevelScreen_UI()
{
	RemoveFromEvent();
}

void EndLevelScreen_UI::Initialize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d,std::shared_ptr<Fonts> fonts)
{
	AddtoEvent();
	
	UI::Initialize(device, contex, cb_vs_matrix_2d, fonts);


	FontsList->AddFont("OpenSans_50", "OpenSans_50.spritefont");
	FontsList->AddFont("OpenSans_12", "OpenSans_12.spritefont");

	//text
	TextLoad();
	



	Background.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	
		Image.INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	
	for (unsigned int i = 0; i < 2; i++) {
		Buttons[i].INITSprite(_Contex.Get(), _Device.Get(), *_cb_vs_matrix_2d);
	}

}

void EndLevelScreen_UI::Update(float dt)
{
	if (ToShow) {
		_TextList.clear();
		if (OnLoad) {
			EventSystem::Instance()->AddEvent(EVENTID::ShowCursorEvent);
			//notifcation for cursor stuff
			EventSystem::Instance()->AddEvent(EVENTID::GamePauseEvent);
			OnLoad = false;
		}

		


		//background
		Background.Function({ 235,209,240 }, { _SizeOfScreen.x,_SizeOfScreen.y }, { 0,0 }, 1.0f);
		//title card

		Image.Function("Title_Card\\TitleCard.png", { static_cast<float>(_SizeOfScreen.x * 0.5),static_cast<float>(_SizeOfScreen.y * 0.12) }, { (_SizeOfScreen.x / 2) - (static_cast<float>(_SizeOfScreen.x * 0.5) / 2),0 });
		AddButtons();


		TextToDraw text;
		text._Text = LoadedTextMap["Level_End_Text"];

		XMVECTOR textsize= FontsList->GetFont("OpenSans_50")->GetSpriteFont()->MeasureString(text._Text.c_str());
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
		Image.Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix);
		for (unsigned int i = 0; i < 2; i++) {
			Buttons[i].Draw(_Contex.Get(), _Device.Get(), *_cb_ps_scene, *_cb_vs_matrix_2d, WorldOrthMatrix, FontsList->GetFont("OpenSans_12").get());
			Shaders::BindShaders(_Contex.Get(), vert, pix);
		}

		for (auto& Text : _TextList)
		{
			FontsList->GetFont("OpenSans_50")->RenderString(Text._Text, Text._Position, Text._Colour);
		}
		
	}
}

void EndLevelScreen_UI::TextLoad()
{
	vector<JSON::TextData>EndLevel_Text = TextLoader::Instance()->LoadText("EndLevel_Text");
	LoadedTextMap = TextLoader::Instance()->ConvertToMap(EndLevel_Text);
	vector<JSON::TextData>EndLevelButtons_Text = TextLoader::Instance()->LoadText("EndLevelButtons_Text");
	map<string,string>tempMap= TextLoader::Instance()->ConvertToMap(EndLevelButtons_Text);
	//join maps
	LoadedTextMap.insert(tempMap.begin(), tempMap.end());
}

void EndLevelScreen_UI::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
		case EVENTID::GameEndLevelEvent:
		{
			ToShow = true;
		
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
			_MouseData.LPress = false;
		}
	}
}

void EndLevelScreen_UI::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->AddClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->AddClient(EVENTID::SetNextLevelEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GameEndLevelEvent, this);
}

void EndLevelScreen_UI::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIKeyInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UIMouseInput, this);
	EventSystem::Instance()->RemoveClient(EVENTID::SetNextLevelEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameEndLevelEvent, this);
}

void EndLevelScreen_UI::AddButtons()
{
	//buttons
	if (Buttons[0].Function(LoadedTextMap["Button_1"], ButtonTex, { _SizeOfScreen.x / 7, _SizeOfScreen.y / 7 }, XMFLOAT2{ (_SizeOfScreen.x / 2) - (_SizeOfScreen.x / 7) - 50, static_cast<float>(_SizeOfScreen.y * 0.55) }, DirectX::Colors::Black, _MouseData)) {
		//go to hub
		EventSystem::Instance()->AddEvent(EVENTID::GameUnPauseEvent);
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &Hub);
		
		ToShow = false;
		OnLoad = true;
	}
	else if (Buttons[1].Function(LoadedTextMap["Button_2"], ButtonTex, { _SizeOfScreen.x / 7, _SizeOfScreen.y / 7 }, XMFLOAT2{ (_SizeOfScreen.x / 2) + 50, static_cast<float>(_SizeOfScreen.y * 0.55) }, DirectX::Colors::Black, _MouseData)) {
		//go to next level
		EventSystem::Instance()->AddEvent(EVENTID::HideCursorEvent);
		EventSystem::Instance()->AddEvent(EVENTID::GameUnPauseEvent);
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, &NextLevel);
		
		ToShow = false;
		OnLoad = true;
	}
}
