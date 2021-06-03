#pragma once
#include "UI.h"
#include<vector>
class Pause :
    public UI
{
public:
	Pause();
	~Pause();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);
	void TextLoad();
	bool GetPause() { return _isPaused; }

	void HandleEvent(Event* event);


private:
	void AddtoEvent();
	void RemoveFromEvent();

	void ButtonCreate();
	void AddTipText();
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
	
	int currentLevel=0;
	UINT32 Hub = 0;

	map<string, string>LoadedTextMap;

	bool OnLoad = true;
};