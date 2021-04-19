#include "stdafx.h"
#include "UI_Manager.h"
#include"Graphics.h"

UI_Manager::UI_Manager()
{
}

void UI_Manager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	UiList["All"]=make_shared<Custom_UI>();
	UiList["All"]->Inizalize(device, context);
}

UI_Manager::~UI_Manager()
{
}

void UI_Manager::Draw(Graphics* level, VertexShader& vert, PixelShader& pix)
{
	
		if (!UiList.empty()) {
			
			if (IsToDraw) {
				UiList["All"]->GameHUD(level);
			}

			if (UiList["All"]->isPaused) {
				UiList["All"]->Pause();
			}

			//end draw
			UiList["All"]->BeginDraw(level,vert, pix);
		}

}

void UI_Manager::HandleEvent(Event* event)
{
}
