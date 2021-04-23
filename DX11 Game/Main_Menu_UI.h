#pragma once
#include "UI.h"
class Main_Menu_UI :
    public UI
{
public:
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d);
	void Update();
	void BeginDraw(VertexShader& vert, PixelShader& pix);

	void HandleEvent(Event* event);


private:
	//MainMenu
	ColourBlock MainMenuBackGround;
	Buttion_Widget MainMenuButtions[5];
	Immage_Widget Titlecard;
};

