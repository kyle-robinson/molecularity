#include "stdafx.h"
#include "UI.h"

#include"Graphics.h"

UI::UI()
{
}

void UI::Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context, Graphics* level)
{
	CustomUiManger = make_shared<Custom_UI>();
	CustomUiManger->Inizalize(level);
}

UI::~UI()
{
}

void UI::Draw(Graphics* level)
{
	if (IsToDraw) {
		
		if (CustomUiManger != nullptr) {
			
			
			CustomUiManger->GameHUD(level);

			if (CustomUiManger->isPaused) {
				CustomUiManger->Pause(level);
			}

			//end draw
			CustomUiManger->BeginDraw(level);
		}

	}

}
