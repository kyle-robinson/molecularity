#pragma once
#ifndef CREDITS_UI_H
#define CREDITS_UI_H

#include "UI.h"

struct ImageData
{
	XMFLOAT2 Size;
	std::string Name;
	std::string FileName;
};

/// <summary>
/// Handles the user interface for the credits sequence.
/// </summary>
class Credits_UI : public UI
{
public:
	Credits_UI();
	~Credits_UI();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts );
	void Update( float dt );
	void BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );

	void TextLoad();
	void HandleEvent( Event* event );
private:
	void AddtoEvent();
	void RemoveFromEvent();

	void LoadImages();
	void AddImmage();
	void AddText();
private:
	// Texture Info
	Image_Widget Images[10];
	ColourBlock Background;
	std::vector<TextToDraw> _TextList;
	std::vector<ImageData> _ImageList;

	// Credit Info
	std::vector<JSON::TextData> CreditsINFO;
	UINT32 LevelTo = 0;
	float NextY = 0;
	float CurrentYPos;
	int imagecount = 0;
};

#endif