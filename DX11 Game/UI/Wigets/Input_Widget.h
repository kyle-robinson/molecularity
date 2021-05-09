#pragma once
#include "widget.h"
template<typename background>
class Input_Widget :
    public widget
{
public:
    Input_Widget();
    ~Input_Widget();

    bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender);



    void Function(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, background colour, XMVECTORF32 textColour, unsigned char key, MouseData MData);
    void setCurrentText(string text) { CurrentText = text; }
    string getCurrentText() {
        return CurrentText;
    }

private:
    Sprite Background;
    background BackgoundColour;

    string CurrentText;
    XMVECTORF32 TextColour;
    bool Selcted=false;

};

template<typename background>
Input_Widget<background>::Input_Widget()
{
}

template<typename background>
Input_Widget<background>::~Input_Widget()
{
}

template<typename background>
bool Input_Widget<background>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	Selcted = false;
	Background.Initialize(Device, Contex, _Size.x, _Size.y, Colour{ 0,0,0 }, cb_vs_matrix_2d);
	return true;
}

template<typename background>
void Input_Widget<background>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{
	Background.UpdateTex(Device, BackgoundColour);
	Background.SetInitialPosition(_Pos.x, _Pos.y, 0);
	Background.SetScale(_Size.x, _Size.y);

	cb_ps_scene.data.alphaFactor = _AlfaFactor;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	Background.Draw(WorldOrthoMatrix);
	XMFLOAT2 textpos = { _Pos.x + 10 ,_Pos.y + (_Size.y / 2) - 12 };
	textrender->RenderString(CurrentText, textpos, TextColour);
}

template<typename background>
void Input_Widget<background>::Function(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, background colour, XMVECTORF32 textColour, unsigned char key, MouseData MData)
{
	_Size = size;
	_Pos = pos;
	BackgoundColour = colour;
	TextColour = textColour;

	//get if user hit
	if (
		MData.Pos.x >= pos.x &&
		MData.Pos.x <= (pos.x + size.x) &&
		MData.Pos.y >= pos.y &&
		MData.Pos.y <= (pos.y + size.y)) {
		if (MData.LPress) {
			Selcted = true;
			CurrentText = "";
		}

	}

	//get text to display
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
		default:
			if (key != 0) {
				CurrentText = key;
				Selcted = false;
			}
			else if (MData.MPress)
			{
				CurrentText = "Scroll Wheel";
				Selcted = false;
			}
			else if (MData.RPress)
			{
				CurrentText = "Right Mouse";
				Selcted = false;
			}
			break;
		}

	}
}


