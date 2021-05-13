#pragma once
#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "widget.h"

template<typename ButtonTexType>
class Button_Widget : public widget
{
public:
    enum ButtonState
    {
        Pressed = 0,
        Hover,
        Default
    };

    Button_Widget() = default;

    bool INITSprite( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d );
    void Draw( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender );

    bool GetIsPressed() const noexcept { return IsPressed; }
    bool Function( string text, vector<ButtonTexType>  ButtonText, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, XMVECTORF32 textColour, MouseData MData );

private:
    Sprite _Background;
    ButtonTexType ButtonColour;
    string ButtonTex;
    XMVECTORF32 TextColour;
    bool IsPressed = false;
    ButtonState CurrentState;
};

template<typename ButtonTexType>
inline bool Button_Widget<ButtonTexType>::INITSprite( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d )
{
    _Background.Initialize( Device, Contex, _Size.x, _Size.y, ButtonColour, cb_vs_matrix_2d );
    return true;
}

template<typename ButtonTexType>
inline void Button_Widget<ButtonTexType>::Draw( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender )
{
    _Background.SetInitialPosition( _Pos.x, _Pos.y, 0 );
    _Background.SetScale( _Size.x, _Size.y );
    _Background.UpdateTex( Device, ButtonColour );
    cb_ps_scene.data.alphaFactor = _AlfaFactor;
    cb_ps_scene.data.useTexture = false;

    if ( !cb_ps_scene.ApplyChanges() ) return;
    Contex->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
    _Background.Draw( WorldOrthoMatrix );

    XMFLOAT2 textpos = { _Pos.x + 10 ,_Pos.y + ( _Size.y / 2 ) - 12 };
    textrender->RenderString( ButtonTex, textpos, TextColour );
}

template<typename ButtonTexType>
inline bool Button_Widget<ButtonTexType>::Function( string text, vector<ButtonTexType> ButtonText, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, XMVECTORF32 textColour, MouseData MData )
{
    CurrentState = Default;
    ButtonTex = text;
    _Size = size;
    _Pos = pos;
    TextColour = textColour;

    // button collision
    if (
        MData.Pos.x >= pos.x &&
        MData.Pos.x <= ( pos.x + size.x ) &&
        MData.Pos.y >= pos.y &&
        MData.Pos.y <= ( pos.y + size.y ) ) {
        if ( MData.LPress ) {
            CurrentState = Pressed;
        }
        else {
            CurrentState = Hover;
        }
    }

    switch ( CurrentState )
    {
    case Pressed:
        // button hit
        ButtonColour = ButtonText[0];
        IsPressed = true;
        return true;
        break;

    case Hover:
        // mouse over button
        ButtonColour = ButtonText[1];
        break;

    case Default:
        // do nothing
        ButtonColour = ButtonText[2];
        break;

    default:
        break;
    }
    IsPressed = false;
    return false;
}

#endif