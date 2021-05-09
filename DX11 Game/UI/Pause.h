#pragma once
#ifndef PAUSE_H
#define PAUSE_H

#include "UI.h"

class Pause : public UI
{
public:
	Pause();
	~Pause();
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	void Update();
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	bool GetPause() { return _isPaused; }
	void HandleEvent(Event* event);

private:
	void CleanUp();
private:

	bool _isPaused;
	//Pause
	Button_Widget<string> PauseButtons[4];
	ColourBlock PauseBackground;
	Image_Widget _TitleCard;
	vector<TextToDraw> PauseTextTitles;
	vector<TextToDraw> PauseTextPG;
	vector<string> ButtonTex = { "Resources\\Textures\\UI_Buttons\\Button_1_Down.dds", 
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds", 
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds" };
	
	//fonts
	std::shared_ptr<TextRenderer>  HeadderTextRenderer;
	std::shared_ptr<TextRenderer>  PGTextRenderer;
};

#endif