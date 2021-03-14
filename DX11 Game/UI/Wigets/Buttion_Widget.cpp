#include "stdafx.h"
#include "Buttion_Widget.h"

Buttion_Widget::Buttion_Widget(string text, vector< Colour>  colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData)
{
	IsPressed=Function(text, colour, size, pos, MData);
	
}

Buttion_Widget::Buttion_Widget(vector<string> tex, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData)
{
	IsPressed = Function( tex, size, pos, MData);
}

bool Buttion_Widget::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	_Bakground.Initialize(Device, Contex, _Size.x, _Size.y, ButtionColour, cb_vs_matrix_2d);
	return false;
}

void Buttion_Widget::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{
	
	_Bakground.UpdateTex(Device, ButtionColour);
	_Bakground.SetInitialPosition(_Pos.x, _Pos.y, 0);
	_Bakground.SetScale(_Size.x, _Size.y);

	cb_ps_scene.data.alphaFactor = _AlfaFactor;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	_Bakground.Draw(WorldOrthoMatrix);

	textrender->RenderString(ButtionTex, _Pos, DirectX::Colors::White);

}

bool Buttion_Widget::Function(string text,vector< Colour> colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData)
{
	 CurrentState = Defaulet;
	 ButtionTex = text;
	_Size = size;
	_Pos = pos;

	//buttion collison
	if (
		MData.Pos.x >= pos.x &&
		MData.Pos.x <= (pos.x + size.x) &&
		MData.Pos.y >= pos.y &&
		MData.Pos.y <= (pos.y + size.y)) {
		if (MData.LPress) {
			CurrentState = Pressed;
		}
		else {
			CurrentState = Hover;
		}
	}


	switch (CurrentState)
	{
	case Pressed:
		//buttion hit
	
		ButtionColour = colour[0];
		IsPressed = true;
		return true;
		break;
	case Hover:
		//mouce over buttion
		ButtionColour = colour[1];
		
		break;
	case Defaulet:
		// do nothing

		ButtionColour = colour[2];

		
		break;
	default:
		
		break;
	}
	IsPressed = false;
	return false;
}

bool Buttion_Widget::Function(vector<string> colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData)
{
	CurrentState = Defaulet;

	_Size = size;
	_Pos = pos;

	//buttion collison
	if (
		MData.Pos.x >= pos.x &&
		MData.Pos.x <= (pos.x + size.x) &&
		MData.Pos.y >= pos.y &&
		MData.Pos.y <= (pos.y + size.y)) {
		if (MData.LPress) {
			CurrentState = Pressed;
		}
		else {
			CurrentState = Hover;
		}
	}


	switch (CurrentState)
	{
	case Pressed:
		//buttion hit

		ButtionTex = colour[0];
		IsPressed = true;
		return true;
		break;
	case Hover:
		//mouce over buttion
		ButtionTex = colour[1];

		break;
	case Defaulet:
		// do nothing

		ButtionTex = colour[2];


		break;
	default:

		break;
	}
	IsPressed = false;
	return false;
}
