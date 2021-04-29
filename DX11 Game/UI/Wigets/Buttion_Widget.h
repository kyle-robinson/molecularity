#pragma once
#include "widget.h"
template<typename ButtionTexType>
class Buttion_Widget :
    public widget
{

public:
    Buttion_Widget() = default;

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

    bool Function(string text, vector<ButtionTexType>  ButtionText, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, XMVECTORF32 textColour, MouseData MData);
 
private:

    Sprite _Bakground;
    ButtionTexType ButtionColour;
    string ButtionTex;
    XMVECTORF32 TextColour;
    bool IsPressed = false;
    ButtionState CurrentState;

};

template<typename ButtionTexType>
inline bool Buttion_Widget<ButtionTexType>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
    _Bakground.Initialize(Device, Contex, _Size.x, _Size.y, ButtionColour, cb_vs_matrix_2d);
    return true;
}

template<typename ButtionTexType>
inline void Buttion_Widget<ButtionTexType>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{
        
    	_Bakground.SetInitialPosition(_Pos.x, _Pos.y, 0);
    	_Bakground.SetScale(_Size.x, _Size.y);
        _Bakground.UpdateTex(Device, ButtionColour);
    	cb_ps_scene.data.alphaFactor = _AlfaFactor;
    	cb_ps_scene.data.useTexture = false;
    
    	if (!cb_ps_scene.ApplyChanges()) return;
    	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
    	_Bakground.Draw(WorldOrthoMatrix);
    
        XMFLOAT2 textpos = { _Pos.x+10 ,_Pos.y+ (_Size.y /2)-12 };
    	textrender->RenderString(ButtionTex, textpos, TextColour);
}

template<typename ButtionTexType>
inline bool Buttion_Widget<ButtionTexType>::Function(string text,vector<ButtionTexType> ButtionText, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, XMVECTORF32 textColour, MouseData MData)
{
        CurrentState = Defaulet;
    	 ButtionTex = text;
    	_Size = size;
    	_Pos = pos;
        TextColour = textColour;
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
    	
    		ButtionColour = ButtionText[0];
    		IsPressed = true;
    		return true;
    		break;
    	case Hover:
    		//mouce over buttion
    		ButtionColour = ButtionText[1];
    		
    		break;
    	case Defaulet:
    		// do nothing
    
    		ButtionColour = ButtionText[2];
    
    		
    		break;
    	default:
    		
    		break;
    	}
    	IsPressed = false;
    	return false;
}
