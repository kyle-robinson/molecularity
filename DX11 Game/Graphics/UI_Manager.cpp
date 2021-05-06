#include "stdafx.h"
#include "UI_Manager.h"
#include"Graphics.h"

#include<UI/HUD_UI.h>
#include<UI/Pause.h>
#include<UI/Settings_Menu_UI.h>


UI_Manager::UI_Manager()
{
	EventSystem::Instance()->AddClient(EVENTID::WorldOrthMatrixEvent, this);
}

void UI_Manager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d)
{


	for (auto const& UIItem : UiList) {

		UIItem.second->Inizalize(device, context, _cb_vs_matrix_2d);
	}
	
}

UI_Manager::~UI_Manager()
{
}

void UI_Manager::Update()
{
	for (auto const& UIItem : UiList)
	{
		UIItem.second->Update();
	}
}

void UI_Manager::Draw(VertexShader& vert, PixelShader& pix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
	for (auto const& UIItem : UiList)
	{
		UIItem.second->BeginDraw(vert,pix, XMLoadFloat4x4(&WorldOrthMatrix), _cb_ps_scene);
	}
		

}

void UI_Manager::AddUi(std::shared_ptr < UI> NewUI, string Name)
{
	UiList[Name] = NewUI;
	
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


	}




}

