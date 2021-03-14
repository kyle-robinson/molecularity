#pragma once
#include "widget.h"
#include"Buttion_Widget.h"
enum DopStae
{
	Down,
	Up
};

template<typename ListData>
class DropDown_Widget :
    public widget
{

public:
	DropDown_Widget() = default;
    DropDown_Widget(vector<ListData>DropDownList,DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos,Colour colour,MouseData MData);
   
	bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
	void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix,TextRenderer* textrender);
    ListData getSelected() { return DataSelected; }


	void Function(vector<ListData>DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour,MouseData MData);

	void setCurrent(int slected){
		Selected = slected;
	}
	

private:
    vector<ListData>_ListData;
    int Selected=0;
    ListData DataSelected;

    Sprite _Bakground;
	Colour BakgroungColour;

    Sprite _Options;

    Buttion_Widget ButtionDrop;
	Buttion_Widget ListButtions [10];
	DopStae DropState=Up;

};

template<typename ListData>
inline DropDown_Widget<ListData>::DropDown_Widget(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour, MouseData MData)
{
	Function(DropDownList, size, pos, colour, MData);
}



template<typename ListData>
inline bool DropDown_Widget<ListData>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	_Bakground.Initialize(Device, Contex, _Size.x, _Size.y, BakgroungColour, cb_vs_matrix_2d);
	ButtionDrop.INITSprite(Contex, Device, cb_vs_matrix_2d);
	for (UINT i = 0; i < 10; i++)
	{
		ListButtions[i].INITSprite(Contex, Device, cb_vs_matrix_2d);
	}
	
	return true;
}

template<typename ListData>
inline void DropDown_Widget<ListData>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender)
{
	_Bakground.UpdateTex(Device, BakgroungColour);
	_Bakground.SetScale(_Size.x, _Size.y);
	_Bakground.SetInitialPosition(_Pos.x, _Pos.y, 0);

	cb_ps_scene.data.alphaFactor = _AlfaFactor;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	_Bakground.Draw(WorldOrthoMatrix);
	
	

	ButtionDrop.Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);

	if (DropState==Down)
	{
		
		for (int i = 0; i < _ListData.size(); i++)
		{


			ListButtions[i].Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
		
		}
		
	}
	

	//text	
	textrender->RenderString(_ListData[Selected], _Pos, DirectX::Colors::White);
	if (DropState == Down)
	{
		for (int i = 0; i < _ListData.size(); i++)
		{
			textrender->RenderString(_ListData[i], ListButtions[i].GetPos(), DirectX::Colors::White);
		}
	}

}

template<typename ListData>
inline void DropDown_Widget<ListData>::Function(vector<ListData> DropDownList, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, Colour colour,MouseData MData)
{
	
	BakgroungColour = colour;
	_Size = size;
	_Pos = pos;
	_ListData = DropDownList;
	
	ButtionDrop.Function("", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { size.y, size.y }, XMFLOAT2{ pos.x + size.x ,  pos.y }, MData);

	//list buttions
	
	switch (DropState)
	{
	case Down: {
		float PosY = pos.y + size.y;
	for (int i = 0; i < DropDownList.size(); i++)
	{
		
		ListButtions[i].Function("", vector<Colour>{ {255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }}, { size.x,size.y }, XMFLOAT2{ pos.x  ,  PosY }, MData);
		PosY += size.y+1;
	}

		for (int i = 0; i < 10; i++)
		{
			

			if (ListButtions[i].GetIsPressed()) {
				Selected = i;
				DropState = Up;
			}
			

		}

		



	}
			 break;
	case Up:
		if (ButtionDrop.GetIsPressed()) {

			DropState = Down;

		}

	default:
		break;
	}

	DataSelected = DropDownList[Selected];
	

	
}
