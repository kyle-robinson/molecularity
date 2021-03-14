#pragma once
#include "widget.h"
class Input_Widget :
    public widget
{
public:
    Input_Widget();
    ~Input_Widget();

    bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender);



    void Function(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour, unsigned char key, MouseData MData);
    void setCurrentText(string text) { CurrentText = text; }
    string getCurrentText() {
        return CurrentText;
    }

private:
    Sprite Background;
    Colour BackgoundColour;

    string CurrentText;

    bool Selcted=false;

};


