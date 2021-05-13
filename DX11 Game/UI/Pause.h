#pragma once
#include "UI.h"
#include<vector>
class Pause :
    public UI
{
public:
	Pause();
	~Pause();
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	void Update();
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	bool GetPause() { return _isPuased; }

	void HandleEvent(Event* event);

private:
	void CleanUp();
private:

	bool _isPuased;
	//Puase
	Buttion_Widget<string> PuaseButtions[4];
	ColourBlock PuaseBakgtound;
	Immage_Widget _TitleCard;
	vector<TextToDraw> PuaseTextTitles;
	vector<TextToDraw> PuaseTextPG;
	vector<string> ButtionTex = { "Resources\\Textures\\UI_Buttions\\Buttion_1_Down.dds", 
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Hover.dds", 
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Up.dds" };
	//fonts
	std::shared_ptr<TextRenderer>  HeadderTextRenderer;
	std::shared_ptr<TextRenderer>  PGTextRenderer;

	
};

