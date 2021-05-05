#pragma once
#include "UI.h"


typedef enum Tabs {
	GrapicsTab,
	GenralTab,
	SoundTab,
	ControlsTab
};

class Settings_Menu_UI :
    public UI
{
public:
	Settings_Menu_UI();
	~Settings_Menu_UI();

	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	void Update();
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	void HandleEvent(Event* event);

private:
	void CreateSettings(JSON::SettingData& settingData);
private:
	bool _isSettings;
	std::vector<JSON::SettingData> _SettingsData;
	//Settings
	Immage_Widget SettingsBakgtound;
	PageSlider_Widget< Colour, Colour> SettingsScrollBar;
	DropDown_Widget<string,string,string> SettingsDropdowns[10];
	DataSlider_Widget<string, string> SettingsSliders[10];
	Buttion_Widget<string> SettingsButtions[10];
	Input_Widget<string> ControllInput[20];
	vector<TextToDraw> PuaseTextTitles;
	vector<TextToDraw> PuaseTextPG;


	vector<string> ButtionTexDrop = { "Resources\\Textures\\Settings\\DropArrow_Blue.dds",
		"Resources\\Textures\\Settings\\DropArrow_Blue.dds",
		"Resources\\Textures\\Settings\\DropArrow.dds" };

	vector<string> ButtionBackDrop = { "Resources\\Textures\\Settings\\Input_Blue.dds",
		"Resources\\Textures\\Settings\\Input_Blue.dds",
		"Resources\\Textures\\Settings\\Input_Yellow.dds" };



	vector<string> ButtionTex = { "Resources\\Textures\\Settings\\Buttion_SettingsBar_Hover.dds",
		"Resources\\Textures\\Settings\\Buttion_SettingsBar_Hover.dds",
		"Resources\\Textures\\Settings\\Buttion_SettingsBar_Up.dds" };

	vector<string> AcceptButtion = { "Resources\\Textures\\UI_Buttions\\Buttion_1_Down.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Hover.dds",
		"Resources\\Textures\\UI_Buttions\\Buttion_1_Up.dds" };

	XMFLOAT2 TabTextPos;

	UINT SettingsDropCount = 0;
	UINT SettingSliderCount = 0;
	UINT SettingsButtionCount = 0;
	UINT SettingsInputCount = 0;

	Tabs CurrentTab = GenralTab;


	std::shared_ptr<TextRenderer>  HeadderTextRenderer;
	std::shared_ptr<TextRenderer>  PGTextRenderer;


	float currentY;
};

