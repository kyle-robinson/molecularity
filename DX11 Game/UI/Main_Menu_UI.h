#pragma once
#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include "UI.h"

class Main_Menu_UI : public UI
{
public:
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d);
	void Update();
	void BeginDraw(VertexShader& vert, PixelShader& pix);
	void HandleEvent(Event* event);

private:
	//MainMenu
	ColourBlock MainMenuBackGround;
	Button_Widget<string> MainMenuButtons[5];
	Image_Widget Titlecard;
};

#endif