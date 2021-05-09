#pragma once
#include "widget.h"



class Immage_Widget :
    public widget
{

public:
    Immage_Widget();
    Immage_Widget(string texture, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos);
    Immage_Widget(ID3D11Resource Texres, ID3D11ShaderResourceView TexView, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos);
    ~Immage_Widget();

    bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix);


    bool Function(string texture, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos);
    
private:

    Sprite _Immage;
    string _TexFile;
    


};

