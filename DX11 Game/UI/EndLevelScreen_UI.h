#pragma once
#ifndef ENDLEVELSCREEN_UI_H
#define ENDLEVELSCREEN_UI_H

#include "UI.h"

/// <summary>
/// Handle the user interface for the splash screen at the end of each level.
/// </summary>
class EndLevelScreen_UI : public UI
{
public:
	EndLevelScreen_UI();
	~EndLevelScreen_UI();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts );
	void Update( float dt );
	void BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );

	void TextLoad();
	void HandleEvent( Event* event );
private:
	void AddtoEvent();
	void RemoveFromEvent();
	void AddButtons();
private:
	bool ToShow;
	bool OnLoad = true;

	Image_Widget Image;
	ColourBlock Background;
	Button_Widget<std::string> Buttons[2];
	std::vector<TextToDraw> _TextList;
	std::unordered_map<std::string, std::string>LoadedTextMap;

	UINT32 Hub = 0;
	UINT32 NextLevel = 0;
	std::vector<std::string> ButtonTex = { "Resources\\Textures\\UI_Buttons\\Button_1_Down.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds" };
};

#endif