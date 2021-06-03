#pragma once
#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include "Widget.h"

/// <summary>
/// Handles the loading and rendering of image widgets.
/// </summary>
class Image_Widget : public Widget
{
public:
    Image_Widget();
    ~Image_Widget();

    bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix);
    bool Function(std::string texture, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos);
private:
    Sprite _Image;
    std::string _TexFile;
};

#endif