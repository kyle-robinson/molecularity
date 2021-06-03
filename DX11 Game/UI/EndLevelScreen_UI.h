#pragma once
#include "UI.h"
class EndLevelScreen_UI :
    public UI
{
public:
	EndLevelScreen_UI();
	~EndLevelScreen_UI();


	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);
	void TextLoad();
	void HandleEvent(Event* event);
private:

	void AddtoEvent();
	void RemoveFromEvent();

	void AddButtons();

private:
	map<string, string>LoadedTextMap;
	Image_Widget Image;
	ColourBlock Background;
	Button_Widget<string> Buttons[2];
	vector<TextToDraw> _TextList;
	bool ToShow;
	UINT32 Hub = 0;
	UINT32 NextLevel = 0;
	vector<string> ButtonTex = { "Resources\\Textures\\UI_Buttons\\Button_1_Down.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds" };

	bool OnLoad= true;
};

