#pragma once
#ifndef PAUSE_H
#define PAUSE_H

#include "UI.h"

/// <summary>
/// Handles the UI systems needed for the pause menu.
/// </summary>
class Pause : public UI
{
public:
	Pause();
	~Pause();
	
	void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts );
	void Update( float dt );
	void BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );
	
	void TextLoad();
	bool GetPause() { return _isPaused; }
	void HandleEvent( Event* event );
private:
	void AddtoEvent();
	void RemoveFromEvent();
	void ButtonCreate();
	void AddTipText();
private:
	bool _isPaused;

	// Pause Menu
	Button_Widget<std::string> PauseButtons[4];
	ColourBlock PauseBackground;
	Image_Widget _TitleCard;
	std::vector<TextToDraw> PauseTextTitles;
	std::vector<TextToDraw> PauseTextPG;
	std::vector<std::string> ButtonTex = { "Resources\\Textures\\UI_Buttons\\Button_1_Down.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds" };

	UINT32 Hub = 0;
	int currentLevel = 0;
	bool OnLoad = true;
	std::unordered_map<std::string, std::string>LoadedTextMap;
};

#endif