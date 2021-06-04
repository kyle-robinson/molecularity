#pragma once
#include "widget.h"
#include "Button_Widget.h"
enum DropState
{
	Down,
	Up
};

template<typename ListData, typename BackgroundType, typename ButtonBacktype>
class DropDown_Widget :
    public widget
{

public:
	DropDown_Widget() = default;
    DropDown_Widget(vector<ListData>DropDownList,DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos,Colour colour, MouseData MData);
   
	bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
	void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix,TextRenderer* textrender, VertexShader& vert, PixelShader& pix);
    ListData getSelected() { return DataSelected; }


	void Function(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, vector<BackgroundType> Backcolour, vector<ButtonBacktype> ButtonImage, XMVECTORF32 textColour, ListData Current, MouseData MData);

	void setCurrent(int selected){
		Selected = selected;
	}
	int GetIntSelcted() { return Selected; }
	bool GetIsDown();

private:
    vector<ListData>_ListData;
    int Selected = 0;
    ListData DataSelected;

    Sprite _Background;
	BackgroundType BackgroundColour;
	int Flag;
	int FlagMax = 20;
    Sprite _Options;
	XMVECTORF32 TextColour;
    Button_Widget<ButtonBacktype> ButtonDrop;
	Button_Widget<BackgroundType> ListButtons [10];
	DropState DropState = Up;

};

template<typename ListData, typename BackgroundType, typename ButtonBacktype>
inline DropDown_Widget<ListData, BackgroundType, ButtonBacktype>::DropDown_Widget(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour, MouseData MData)
{
	Function(DropDownList, size, pos, colour, MData);
}



template<typename ListData, typename BackgroundType, typename ButtonBacktype>
inline bool DropDown_Widget<ListData, BackgroundType, ButtonBacktype>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	Flag = FlagMax;
	_Background.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
	ButtonDrop.INITSprite(Contex, Device, cb_vs_matrix_2d);
	for (UINT i = 0; i < 10; i++)
	{
		ListButtons[i].INITSprite(Contex, Device, cb_vs_matrix_2d);
	}
	
	return true;
}

template<typename ListData, typename BackgroundType, typename ButtonBacktype>
inline void DropDown_Widget<ListData, BackgroundType, ButtonBacktype>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender, VertexShader& vert, PixelShader& pix)
{
	_Background.UpdateTex(Device, "Resources\\Textures\\Settings\\Input_Yellow.dds");
	_Background.SetScale(_Size.x, _Size.y);
	_Background.SetInitialPosition(_Pos.x, _Pos.y, 0);

	cb_ps_scene.data.alphaFactor = _AlphaFactor;
	cb_ps_scene.data.useTexture = true;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	_Background.Draw(WorldOrthoMatrix);
	
	

	ButtonDrop.Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
	Shaders::BindShaders(Contex, vert, pix);
	if (DropState== DropState::Down)
	{
		
		for (int i = 0; i < _ListData.size(); i++)
		{


			ListButtons[i].Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
			Shaders::BindShaders(Contex, vert, pix);
		}
		
	}
	
	XMVECTOR textsize = textrender->GetSpriteFont()->MeasureString(_ListData[Selected].c_str());
	XMFLOAT2 textpos = { _Pos.x + (_Size.x / 2) - (DirectX::XMVectorGetX(textsize) * textrender->GetScale().x) / 2 ,_Pos.y + (_Size.y / 2) - (DirectX::XMVectorGetY(textsize) * textrender->GetScale().y) / 2 };
	//text	
	textrender->RenderString(_ListData[Selected], textpos, TextColour);
	

}

template<typename ListData, typename BackgroundType, typename ButtonBacktype>
inline void DropDown_Widget< ListData,  BackgroundType,  ButtonBacktype>::Function(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, vector<BackgroundType> Backcolour, vector<ButtonBacktype> ButtonImage, XMVECTORF32 textColour, ListData Current, MouseData MData)
{
	
	BackgroundColour = Backcolour[2];
	_Size = size;
	_Pos = pos;
	_ListData = DropDownList;
	TextColour = textColour;
	ButtonDrop.Function("", ButtonImage, { size.y, size.y }, XMFLOAT2{ pos.x + size.x ,  pos.y }, textColour, MData);

	for (UINT i = 0; i < _ListData.size(); i++)
	{
		if (Current == _ListData[i]) {
			Selected = i;
		}
	}

	//list buttons
	switch (DropState)
	{
	case DropState::Down: {
		float PosY = pos.y + size.y;
		for (int i = 0; i < DropDownList.size(); i++)
		{
		
			
			if (ListButtons[i].Function(_ListData[i], Backcolour, { size.x,size.y }, XMFLOAT2{ pos.x  ,  PosY }, textColour, MData)) {
				Selected = i;
				DropState = Up;
				Flag = 0;
			}
			PosY += size.y+1;
		}

		if (ButtonDrop.GetIsPressed() && Flag == FlagMax) {

			DropState = Up;
			Flag = 0;
		}
		else if(Flag<FlagMax)
		{
			Flag++;
		}
	}
			 break;
	case DropState::Up:
		if (ButtonDrop.GetIsPressed() && Flag == FlagMax) {

			DropState = Down;
			Flag = 0;
		}
		else if (Flag < FlagMax)
		{
			Flag++;
		}

	default:
		break;
	}

	DataSelected = DropDownList[Selected];
	

	
}

template<typename ListData, typename BackgroundType, typename ButtonBacktype>
inline bool DropDown_Widget<ListData, BackgroundType, ButtonBacktype>::GetIsDown()
{
	if (DropState == Down) {
		return true;
	}
	else
	{
		return false;
	}

	
}