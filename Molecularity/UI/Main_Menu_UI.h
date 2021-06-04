#pragma once
#include "UI.h"
class Main_Menu_UI :
    public UI
{
public:
	Main_Menu_UI();
	~Main_Menu_UI();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);
	void TextLoad();
	void HandleEvent(Event* event);
private:

	void AddtoEvent();
	void RemoveFromEvent();

	void MenuButtons();
	void LinkButtons();
private:
	//MainMenu
	ColourBlock MainMenuBackground;
	Button_Widget<std::string> MainMenuButtons[5];
	Image_Widget Titlecard;

	unordered_map<string, string> LoadedTextMap;
	//github link
	bool openlink = false;
	bool open = true;


	UINT32 LevelTo=0;
	bool IsSettings;
	//button state textures
	vector<string> ButtonTex = { "Resources\\Textures\\UI_Buttons\\Button_1_Down.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds" };



	//Logo from https://github.com/logos
	vector<string> ButtonTex2 = { "Resources\\Textures\\UI_Buttons\\GitHubLogo.png",
		"Resources\\Textures\\UI_Buttons\\GitHubLogo.png",
		"Resources\\Textures\\UI_Buttons\\GitHubLogo.png" };


	bool mouseLoad = true;
};

