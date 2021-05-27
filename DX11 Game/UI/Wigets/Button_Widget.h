#pragma once
#include "widget.h"

template<typename ButtonTexType>
class Button_Widget :
    public widget
{

public:
    Button_Widget() = default;

    bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender);

    bool GetIsPressed() {
        return IsPressed;
    }

    enum ButtonState
    {
        Pressed = 0,
        Hover,
        Default
    };

    bool Function(std::string text, vector<ButtonTexType>  ButtonText, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, XMVECTORF32 textColour,MouseData MData);
 
private:

    Sprite _Background;
    ButtonTexType ButtonColour;
    std::string ButtonTex;
    XMVECTORF32 TextColour;
    bool IsPressed = false;
    ButtonState CurrentState;


};

template<typename ButtonTexType>
inline bool Button_Widget<ButtonTexType>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
    _Background.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
    return true;
}

template<typename ButtonTexType>
inline void Button_Widget<ButtonTexType>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{
        
    	_Background.SetInitialPosition(_Pos.x, _Pos.y, 0);
    	_Background.SetScale(_Size.x, _Size.y);
        
            _Background.UpdateTex(Device, ButtonColour);
       
       
    	cb_ps_scene.data.alphaFactor = _AlphaFactor;
    	cb_ps_scene.data.useTexture = false;
    
    	if (!cb_ps_scene.ApplyChanges()) return;
    	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
    	_Background.Draw(WorldOrthoMatrix);
    

        XMVECTOR textsize = textrender->GetSpriteFont()->MeasureString(ButtonTex.c_str());

        XMFLOAT2 textpos = { _Pos.x + (_Size.x / 2) - (DirectX::XMVectorGetX(textsize) * textrender->GetScale().x) / 2 ,_Pos.y + (_Size.y / 2) - (DirectX::XMVectorGetY(textsize) * textrender->GetScale().y) / 2 };
 
        textrender->RenderString(ButtonTex, textpos, TextColour);
        

    	
}

template<typename ButtonTexType>
inline bool Button_Widget<ButtonTexType>::Function(std::string text,vector<ButtonTexType> ButtonText, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, XMVECTORF32 textColour, MouseData MData)
{
        CurrentState = Default;


      

            ButtonTex = text;
        
       
    	_Size = size;
    	_Pos = pos;
        TextColour = textColour;
    	//button collison
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
    		//button hit
    	
    		ButtonColour = ButtonText[0];
    		IsPressed = true;
            Sound::Instance()->PlaySoundEffect( "MenuClick" );
    		return true;
    		break;
    	case Hover:
    		//mouce over button
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
