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
	unsigned char getKey() {
		return _Key;
	}

	void SetKey(unsigned char key);
private:
    Sprite Background;
    background BackgoundColour;
	unsigned char _Key;
    string CurrentText;
    XMVECTORF32 TextColour;
    bool Selcted = false;

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
	Background.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
	return true;
}

template<typename background>
void Input_Widget<background>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{
	Background.UpdateTex(Device, BackgoundColour);
	Background.SetInitialPosition(_Pos.x, _Pos.y, 0);
	Background.SetScale(_Size.x, _Size.y);

	cb_ps_scene.data.alphaFactor = _AlphaFactor;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	Background.Draw(WorldOrthoMatrix);
	XMVECTOR textsize = textrender->GetSpriteFont()->MeasureString(CurrentText.c_str());
	XMFLOAT2 textpos = { _Pos.x + (_Size.x / 2) - (DirectX::XMVectorGetX(textsize) * textrender->GetScale().x) / 2 ,_Pos.y + (_Size.y / 2) - (DirectX::XMVectorGetY(textsize) * textrender->GetScale().y) / 2 };
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
	
		SetKey(key);
	}
}

template<typename background>
inline void Input_Widget<background>::SetKey(unsigned char key)
{
	switch (key)
	{
	//other keys
	case VK_RETURN:
		CurrentText = "Return";
		Selcted = false;
		_Key = key;
		break;
	case VK_SPACE:
		CurrentText = "Space";
		Selcted = false;
		_Key = key;
		break;
	case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
		CurrentText = "Shift";
		Selcted = false;
		_Key = key;
		break;
	case VK_CONTROL:
		CurrentText = "Ctrl";
		Selcted = false;
		_Key = key;
		break;
		//alt
	case VK_MENU:
		CurrentText = "alt";
		Selcted = false;
		_Key = key;
		break;
	case VK_PAUSE:
		CurrentText = "Pause";
		Selcted = false;
		_Key = key;
		break;
	case VK_ESCAPE:
		CurrentText = "ESC";
		Selcted = false;
		_Key = key;
		break;
	case VK_CAPITAL:
		CurrentText = "CAPS LOCK";
		Selcted = false;
		_Key = key;
		break;
	case VK_BACK:
		CurrentText = "BACKSPACE";
		Selcted = false;
		_Key = key;
		break;
	case VK_TAB:
		CurrentText = "Tab";
		Selcted = false;
		_Key = key;
		break;
		//page up
	case VK_PRIOR:
		CurrentText = "page Up";
		Selcted = false;
		_Key = key;
		break;
		//page down
	case VK_NEXT:
		CurrentText = "page Down";
		Selcted = false;
		_Key = key;
		break;
	case VK_END:
		CurrentText = "End";
		Selcted = false;
		_Key = key;
		break;
	case VK_HOME:
		CurrentText = "Home";
		Selcted = false;
		_Key = key;
		break;
	case VK_SELECT:
		CurrentText = "Select";
		Selcted = false;
		_Key = key;
		break;
	case VK_INSERT:
		CurrentText = "Ins";
		Selcted = false;
		_Key = key;
		break;
	case VK_DELETE:
		CurrentText = "Del";
		Selcted = false;
		_Key = key;
		break;
	case VK_HELP:
		CurrentText = "Help";
		Selcted = false;
		_Key = key;
		break;
	case VK_SCROLL:
		CurrentText = "Scr Lock";
		Selcted = false;
		_Key = key;
		break;
	case VK_CLEAR:
		CurrentText = "Clear";
		Selcted = false;
		_Key = key;
		break;
	//arrow keys
	case VK_UP:
		CurrentText = "Up Arrow";
		Selcted = false;
		_Key = key;
		break;
	case VK_DOWN:
		CurrentText = "Down Arrow";
		Selcted = false;
		_Key = key;
		break;
	case VK_LEFT:
		CurrentText = "Left Arrow";
		Selcted = false;
		_Key = key;
		break;
	case VK_RIGHT:
		CurrentText = "Right Arrow";
		Selcted = false;
		_Key = key;
		break;


	//F keys
	case VK_F1:
		CurrentText = "F1";
		Selcted = false;
		_Key = key;
		break;
	case VK_F2:
		CurrentText = "F2";
		Selcted = false;
		_Key = key;
		break;
	case VK_F3:
		CurrentText = "F3";
		Selcted = false;
		_Key = key;
		break;
	case VK_F4:
		CurrentText = "F4";
		Selcted = false;
		_Key = key;
		break;
	case VK_F5:
		CurrentText = "F5";
		Selcted = false;
		_Key = key;
		break;
	case VK_F6:
		CurrentText = "F6";
		Selcted = false;
		_Key = key;
		break;
	case VK_F7:
		CurrentText = "F7";
		Selcted = false;
		_Key = key;
		break;
	case VK_F8:
		CurrentText = "F8";
		Selcted = false;
		_Key = key;
		break;
	case VK_F9:
		CurrentText = "F9";
		Selcted = false;
		_Key = key;
		break;
	case VK_F10:
		CurrentText = "F10";
		Selcted = false;
		_Key = key;
		break;
	case VK_F11:
		CurrentText = "F11";
		Selcted = false;
		_Key = key;
		break;
	case VK_F12:
		CurrentText = "F12";
		Selcted = false;
		_Key = key;
		break;
	
	//numpad
	case VK_NUMLOCK:
		CurrentText = "Numlock";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD0:
		CurrentText = "Numpad 0";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD1:
		CurrentText = "Numpad 1";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD2:
		CurrentText = "Numpad 2";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD3:
		CurrentText = "Numpad 3";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD4:
		CurrentText = "Numpad 4";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD5:
		CurrentText = "Numpad 5";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD6:
		CurrentText = "Numpad 6";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD7:
		CurrentText = "Numpad 7";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD8:
		CurrentText = "Numpad 8";
		Selcted = false;
		_Key = key;
		break;
	case VK_NUMPAD9:
		CurrentText = "Numpad 9";
		Selcted = false;
		_Key = key;
		break;
	
	
	//math keys
	case VK_MULTIPLY:
		CurrentText = "*";
		Selcted = false;
		_Key = key;
		break;
	case VK_ADD:
		CurrentText = "+";
		Selcted = false;
		_Key = key;
		break;
	case VK_SEPARATOR:
		CurrentText = "";
		Selcted = false;
		_Key = key;
		break;
	case VK_SUBTRACT:
		CurrentText = "-";
		Selcted = false;
		_Key = key;
		break;
	case VK_DECIMAL:
		CurrentText = ".";
		Selcted = false;
		_Key = key;
		break;
	case VK_DIVIDE:
		CurrentText = "/";
		Selcted = false;
		_Key = key;
		break;

	//number key codes
	//Braket left
	case 219:
		CurrentText = "[";
		Selcted = false;
		_Key = key;
		break;
	//Braket right
	case 221:
		CurrentText = "]";
		Selcted = false;
		_Key = key;
		break;
	
	case 222:
		CurrentText = "#";
		Selcted = false;
		_Key = key;
		break;
	case 186:
		CurrentText = ";";
		Selcted = false;
		_Key = key;
		break;
	case 192:
		CurrentText = "'";
		Selcted = false;
		_Key = key;
	case 188:
		CurrentText = ",";
		Selcted = false;
		_Key = key;
		break;
	case 187:
		CurrentText = "=";
		Selcted = false;
		_Key = key;
		break;
	case 223:
		CurrentText = "`";
		Selcted = false;
		_Key = key;
		break;
	case 220:
		CurrentText = "\\";
		Selcted = false;
		_Key = key;
		break;
	case 191:
		CurrentText = "/";
		Selcted = false;
		_Key = key;
		break;
	case 190:
		CurrentText = ".";
		Selcted = false;
		_Key = key;
		break;

	//all other keys
	default:
		if (key != 0) {
			CurrentText = key;
			Selcted = false;
			_Key = key;
		}
		break;
	}
}


