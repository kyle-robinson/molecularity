#pragma once
#include "widget.h"
class ColourBlock :
    public widget
{

public:
    ColourBlock();
    ColourBlock(Colour colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos,float AFactor=1.0f);
    ~ColourBlock();

   bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
   void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene,ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix);

   bool Function(Colour colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos,float AFactor);


private:

    Sprite _ColourSprite;
    Colour _Colour;



};

