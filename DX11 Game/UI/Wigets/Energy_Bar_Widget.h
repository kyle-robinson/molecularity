#pragma once
#include "widget.h"  
/// <summary>
/// Sets up the ablity to show a data bar 
/// </summary>
template<typename BackGroundType, typename BarType, typename FrontType>
class Energy_Bar_Widget :
    public widget
{

public:
	Energy_Bar_Widget(){}
    Energy_Bar_Widget(BackGroundType TexBackGround, BarType TexBar, FrontType TexFront, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, float fraction);

	bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device,ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
	~Energy_Bar_Widget() {}
  
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix);
    void Function(BackGroundType TexBackGround, BarType TexBar, FrontType TexFront, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, float fraction);



	int GetCurrentPercent() { return CurrentPercent; }

private:

    float currentFraction;
    int CurrentPercent;

	//Background 
    Sprite _Background;
    BackGroundType _TypeBackGround;
	

	//Moving bar
    Sprite Bar;
    BarType _TypeBar;
	
	//Front
    Sprite Front;
    FrontType _TypeFront;
	


};
//functions
template<typename BackGroundType, typename BarType, typename FrontType>
inline Energy_Bar_Widget<BackGroundType, BarType, FrontType>::Energy_Bar_Widget(BackGroundType TexBackGround, BarType TexBar, FrontType TexFront, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, float fraction)
{
	Function(TexBackGround, TexBar, TexFront, size, pos, fraction);
}

template<typename BackGroundType, typename BarType, typename FrontType>
inline bool Energy_Bar_Widget<BackGroundType, BarType, FrontType>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{

	_Background.Initialize(Device, Contex, 100, 100, "", cb_vs_matrix_2d);
	Bar.Initialize(Device, Contex,200, 300, "", cb_vs_matrix_2d);
	Front.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);

	return true;
}

template<typename BackGroundType, typename BarType, typename FrontType>
inline void Energy_Bar_Widget<BackGroundType, BarType, FrontType>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix)
{
	//background
	
		_Background.UpdateTex(Device, _TypeBackGround);
	
	_Background.SetInitialPosition(_Pos.x, _Pos.y, 0);
	_Background.SetScale(_Size.x, _Size.y,0 );
	

	cb_ps_scene.data.alphaFactor = 0.5f;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	_Background.Draw(WorldOrthoMatrix);

	//Bar
	
		Bar.UpdateTex(Device, _TypeBar);
	
	Bar.SetInitialPosition(_Pos.x, _Pos.y, 0);
	Bar.SetScale(_Size.x * currentFraction, _Size.y, 0);
	cb_ps_scene.data.alphaFactor = 1.0f;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	Bar.Draw(WorldOrthoMatrix);

	//Front
	if (_TypeFront!="") {
		
			Front.UpdateTex(Device, _TypeFront);
		
		Front.SetInitialPosition(_Pos.x, _Pos.y, 0);
		Front.SetScale(_Size.x+13.5f, _Size.y, 0);

		cb_ps_scene.data.alphaFactor = 1.0f;
		cb_ps_scene.data.useTexture = true;


		if (!cb_ps_scene.ApplyChanges()) return;
		Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
		Front.Draw(WorldOrthoMatrix);
	}
}

template<typename BackGroundType, typename BarType, typename FrontType>
inline void Energy_Bar_Widget<BackGroundType, BarType, FrontType>::Function(BackGroundType TexBackGround, BarType TexBar, FrontType TexFront, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, float fraction)
{
	
	
	
		_TypeBackGround = TexBackGround;
		_TypeBar = TexBar;
		_TypeFront = TexFront;
	

	_Size = size;
	_Pos = pos;

	currentFraction = fraction / 100;
	CurrentPercent = fraction;

}
