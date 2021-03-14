#pragma once
#include "widget.h"
class Buttion_Widget :
    public widget
{

public:
    Buttion_Widget() = default;
    Buttion_Widget(string text, vector<Colour>  colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData);
    Buttion_Widget(vector<string> tex, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData);
    
    bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender);

    bool GetIsPressed() {
        return IsPressed;
    }

    enum ButtionState
    {
        Pressed = 0,
        Hover,
        Defaulet
    };

    bool Function(string text, vector< Colour>  colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData);
    bool Function( vector<string>  colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, MouseData MData);

private:

    Sprite _Bakground;
    Colour ButtionColour;
    
    string ButtionTex;

    bool IsPressed = false;
    ButtionState CurrentState;

};

