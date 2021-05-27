#pragma once
#include "UI.h"
class Main_Menu_UI :
    public UI
{
public:
	Main_Menu_UI();
	~Main_Menu_UI();
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);
	void TextLoad();
	void HandleEvent(Event* event);
private:

	void AddtoEvent();
	void RemoveFromEvent();

	void MenuButtions();
	void LinkButtions();
private:
	//MainMenu
	ColourBlock MainMenuBackground;
	Buttion_Widget<std::string> MainMenuButtions[5];
	Immage_Widget Titlecard;

	map<string, string> LoadedTextMap;
	//github link
	 bool openlink = false;
	 bool open = true;


	UINT32 LevelTo=0;
	bool IsSettings;
	//buttion state textures
	vector<string> ButtionTex = { "Resources\\Textures\\UI_Buttions\\Buttion_1_Down.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Hover.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Up.dds" };



	//Logo from https://github.com/logos
	vector<string> ButtionTex2 = { "Resources\\Textures\\UI_Buttions\\GitHubLogo.png",
		"Resources\\Textures\\UI_Buttions\\GitHubLogo.png",
		"Resources\\Textures\\UI_Buttions\\GitHubLogo.png" };
};

