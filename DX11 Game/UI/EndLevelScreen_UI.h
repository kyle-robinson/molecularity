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

	void AddButtions();

private:
	map<string, string>LoadedTextMap;
	Immage_Widget Image;
	ColourBlock Background;
	Buttion_Widget<string> Buttions[2];
	vector<TextToDraw> _TextList;
	bool ToShow;
	UINT32 Hub = 2;
	UINT32 NextLevel = 0;
	vector<string> ButtionTex = { "Resources\\Textures\\UI_Buttions\\Buttion_1_Down.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Hover.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Up.dds" };

	
};

