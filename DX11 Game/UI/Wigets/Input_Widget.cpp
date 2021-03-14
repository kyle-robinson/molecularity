#include "stdafx.h"
#include "Input_Widget.h"

Input_Widget::Input_Widget()
{
}

Input_Widget::~Input_Widget()
{
}

bool Input_Widget::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{

	Background.Initialize(Device, Contex, _Size.x, _Size.y, Colour{ 0,0,0 } , cb_vs_matrix_2d);
	return true;
}

void Input_Widget::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{

	Background.UpdateTex(Device, BackgoundColour);
	Background.SetInitialPosition(_Pos.x, _Pos.y, 0);
	Background.SetScale(_Size.x, _Size.y);

	cb_ps_scene.data.alphaFactor = _AlfaFactor;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	Background.Draw(WorldOrthoMatrix);

	textrender->RenderString(CurrentText, _Pos, DirectX::Colors::White);

}


void Input_Widget::Function(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour, unsigned char key, MouseData MData)
{

	_Size = size;
	_Pos = pos;
	BackgoundColour = colour;



	if (
		MData.Pos.x >= pos.x &&
		MData.Pos.x <= (pos.x + size.x) &&
		MData.Pos.y >= pos.y &&
		MData.Pos.y <= (pos.y + size.y)) {
		if (MData.LPress) {
			Selcted = true;
		}
		
	}

	if (Selcted) {
		switch (key)
		{
			//Specialkeys
		case VK_RETURN:
			CurrentText = "Enter";
			Selcted = false;
			break;
		case VK_SPACE:
			CurrentText = "Space";
			Selcted = false;
			break;
		case VK_LSHIFT:
			CurrentText = "Left shift";
			Selcted = false;
			break;
		case VK_UP:
			CurrentText = "Up_Arrow";
			Selcted = false;
			break;
		case VK_DOWN:
			CurrentText = "Down_Arrow";
			Selcted = false;
			break;
		case VK_LEFT:
			CurrentText = "Left_Arrow";
			Selcted = false;
			break;
		case VK_RIGHT:
			CurrentText = "Right_Arrow";
			Selcted = false;
			break;
		case 0:
			break;
		
		default:
			
			CurrentText = key;
				
			
			Selcted = false;
			break;
		}

	}
}
