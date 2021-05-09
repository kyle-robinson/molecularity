#pragma once
#include "widget.h"
#include"Buttion_Widget.h"
enum DopStae
{
	Down,
	Up
};

template<typename ListData, typename BakgroundType, typename ButtionBacktype>
class DropDown_Widget :
    public widget
{

public:
	DropDown_Widget() = default;
    DropDown_Widget(vector<ListData>DropDownList,DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos,Colour colour, MouseData MData);
   
	bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
	void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix,TextRenderer* textrender, VertexShader& vert, PixelShader& pix);
    ListData getSelected() { return DataSelected; }


	void Function(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, vector<BakgroundType> Backcolour, vector<ButtionBacktype> ButtionImmage, XMVECTORF32 textColour, ListData Current, MouseData MData);

	void setCurrent(int slected){
		Selected = slected;
	}
	int GetIntSelcted() { return Selected; }
	bool GetIsDown();

private:
    vector<ListData>_ListData;
    int Selected=0;
    ListData DataSelected;

    Sprite _Bakground;
	BakgroundType BakgroungColour;
	int Flag;
	int FlagMax = 20;
    Sprite _Options;
	XMVECTORF32 TextColour;
    Buttion_Widget<ButtionBacktype> ButtionDrop;
	Buttion_Widget<BakgroundType> ListButtions [10];
	DopStae DropState=Up;

};

template<typename ListData, typename BakgroundType, typename ButtionBacktype>
inline DropDown_Widget<ListData, BakgroundType, ButtionBacktype>::DropDown_Widget(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour, MouseData MData)
{
	Function(DropDownList, size, pos, colour, MData);
}



template<typename ListData, typename BakgroundType, typename ButtionBacktype>
inline bool DropDown_Widget<ListData, BakgroundType, ButtionBacktype>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	Flag = FlagMax;
	_Bakground.Initialize(Device, Contex, _Size.x, _Size.y, BakgroungColour, cb_vs_matrix_2d);
	ButtionDrop.INITSprite(Contex, Device, cb_vs_matrix_2d);
	for (UINT i = 0; i < 10; i++)
	{
		ListButtions[i].INITSprite(Contex, Device, cb_vs_matrix_2d);
	}
	
	return true;
}

template<typename ListData, typename BakgroundType, typename ButtionBacktype>
inline void DropDown_Widget<ListData, BakgroundType, ButtionBacktype>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender, VertexShader& vert, PixelShader& pix)
{
	_Bakground.UpdateTex(Device, "Resources\\Textures\\Settings\\Input_Yellow.dds");
	_Bakground.SetScale(_Size.x, _Size.y);
	_Bakground.SetInitialPosition(_Pos.x, _Pos.y, 0);

	cb_ps_scene.data.alphaFactor = _AlfaFactor;
	cb_ps_scene.data.useTexture = true;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	_Bakground.Draw(WorldOrthoMatrix);
	
	

	ButtionDrop.Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
	Shaders::BindShaders(Contex, vert, pix);
	if (DropState== DopStae::Down)
	{
		
		for (int i = 0; i < _ListData.size(); i++)
		{


			ListButtions[i].Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
			Shaders::BindShaders(Contex, vert, pix);
		}
		
	}
	
	XMFLOAT2 textpos = { _Pos.x + 10 ,_Pos.y + (_Size.y / 2) - 12 };
	//text	
	textrender->RenderString(_ListData[Selected], textpos, TextColour);
	

}

template<typename ListData, typename BakgroundType, typename ButtionBacktype>
inline void DropDown_Widget< ListData,  BakgroundType,  ButtionBacktype>::Function(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, vector<BakgroundType> Backcolour, vector<ButtionBacktype> ButtionImmage, XMVECTORF32 textColour, ListData Current, MouseData MData)
{
	
	BakgroungColour = Backcolour[2];
	_Size = size;
	_Pos = pos;
	_ListData = DropDownList;
	TextColour = textColour;
	ButtionDrop.Function("", ButtionImmage, { size.y, size.y }, XMFLOAT2{ pos.x + size.x ,  pos.y }, textColour, MData);

	for (UINT i = 0; i < _ListData.size(); i++)
	{
		if (Current == _ListData[i]) {
			Selected = i;
		}
	}

	//list buttions
	switch (DropState)
	{
	case DopStae::Down: {
		float PosY = pos.y + size.y;
		for (int i = 0; i < DropDownList.size(); i++)
		{
		
			
			if (ListButtions[i].Function(_ListData[i], Backcolour, { size.x,size.y }, XMFLOAT2{ pos.x  ,  PosY }, textColour, MData)) {
				Selected = i;
				DropState = Up;
				Flag = 0;
			}
			PosY += size.y+1;
		}

		if (ButtionDrop.GetIsPressed() && Flag == FlagMax) {

			DropState = Up;
			Flag = 0;
		}
		else if(Flag<FlagMax)
		{
			Flag++;
		}
	}
			 break;
	case DopStae::Up:
		if (ButtionDrop.GetIsPressed() && Flag == FlagMax) {

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

template<typename ListData, typename BakgroundType, typename ButtionBacktype>
inline bool DropDown_Widget<ListData, BakgroundType, ButtionBacktype>::GetIsDown()
{
	if (DropState == Down) {
		return true;
	}
	else
	{
		return false;
	}

	
}