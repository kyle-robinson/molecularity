#pragma once
#include "UI.h"
class Main_Menu_UI :
    public UI
{
public:
	Main_Menu_UI();
	~Main_Menu_UI();
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	void HandleEvent(Event* event);


private:
	//MainMenu
	std::shared_ptr<TextRenderer>  PGTextRenderer;
	ColourBlock MainMenuBackGround;
	Buttion_Widget<std::string> MainMenuButtions[5];
	Immage_Widget Titlecard;

	UINT32 LevelTo=0;
	bool IsSettings;
	//buttion state textures
	vector<string> ButtionTex = { "Resources\\Textures\\UI_Buttions\\Buttion_1_Down.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Hover.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Up.dds" };
};

