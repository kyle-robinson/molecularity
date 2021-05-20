#include "stdafx.h"
#include "UI_Manager.h"
#include"Graphics.h"

#include<UI/HUD_UI.h>
#include<UI/Pause.h>
#include<UI/Settings_Menu_UI.h>


UI_Manager::UI_Manager()
{
	EventSystem::Instance()->AddClient(EVENTID::WorldOrthMatrixEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::RemoveUIItemEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::InputPassEvent, this);
}

void UI_Manager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d)
{


	for (auto const& UIItem : UiList) {

		UIItem.second->SetSizeOfScreen(WinSize);
		UIItem.second->Inizalize(device, context, _cb_vs_matrix_2d);
		
	}
	
}

UI_Manager::~UI_Manager()
{
}

void UI_Manager::Update(float dt)
{
	for (auto const& UIItem : UiList)
	{
		bool ToDraw = false;
		for (int i = 0; i < UiToDraw.size(); i++)
		{
			if (UIItem.first == UiToDraw[i]) {
				ToDraw = true;
			}

		}
		if (ToDraw) {
			UIItem.second->Update(dt);
		}
	}


}

void UI_Manager::Draw(VertexShader& vert, PixelShader& pix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	for (auto const& UIItem : UiList)
	{
		bool ToDraw=false;
		for (int i = 0; i < UiToDraw.size(); i++)
		{
			if (UIItem.first == UiToDraw[i]) {
				ToDraw = true;
			}

		}
		if (ToDraw) {
			UIItem.second->BeginDraw(vert, pix, XMLoadFloat4x4(&WorldOrthMatrix), _cb_ps_scene);
		}
	}
		

}

std::shared_ptr<UI> UI_Manager::GetCustomUi(std::string UIName)
{

	for (auto const& UIItem : UiList)
	{
		if (UIItem.first == UIName) {
			return UiList[UIName];
		}
	}
	return nullptr;
}

void UI_Manager::AddUi(std::shared_ptr < UI> NewUI, string Name)
{
	//check if it is in list
	bool ToAdd = true;
	for (auto const& UIItem : UiList)
	{
		if (UIItem.first == Name) {
			ToAdd = false;
			break;
		}
	}
	if (ToAdd) {
		UiList[Name] = NewUI;
		UiToDraw.push_back(Name);
	}
	
}

void UI_Manager::RemoveUI(std::string Name)
{
	for (auto i = UiList.begin(); i != UiList.end(); i++) {
		if (i->first == Name) {
				//Delete the client in question. No need to keep going since its a unique and more cant exist
				i = UiList.erase(i);
				break;
		}
	}
}

void UI_Manager::RemoveAllUI()
{
	UiList.clear();
}

void UI_Manager::HandleEvent(Event* event)
{

	switch (event->GetEventID())
	{
	case EVENTID::WorldOrthMatrixEvent:
	{
		WorldOrthMatrix = *(XMFLOAT4X4*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		WinSize = *static_cast<XMFLOAT2*>(event->GetData());
	}
	break;
	case EVENTID::RemoveUIItemEvent:
	{
		RemoveUI(*static_cast<string*>(event->GetData()));
	}
	break;

	}




}

void UI_Manager::HideAllUI()
{
	for (auto const& UIItem : UiList)
	{

		vector<string>::iterator iter = UiToDraw.begin();
		while (iter != UiToDraw.end()) {
			if (*iter == UIItem.first) {
				iter = UiToDraw.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void UI_Manager::ShowAllUi()
{
	for (auto const& UIItem : UiList)
	{
		UiToDraw.push_back(UIItem.first);
	}

}

void UI_Manager::ShowUi(string Name)
{
	UiToDraw.push_back(Name);
}

void UI_Manager::HideUi(string Name)
{

	vector<string>::iterator iter = UiToDraw.begin();
	while (iter != UiToDraw.end()) {
		if (*iter == Name) {
			iter = UiToDraw.erase(iter);
		}
		else
		{
			++iter;
		}
	}

}

