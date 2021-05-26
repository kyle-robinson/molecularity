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

	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);
	void TextLoad();
	void HandleEvent(Event* event);

private:
	void CreateSettings(JSON::SettingData& settingData);
	void AddtoEvent();
	void RemoveFromEvent();

	void PageSlider();
	void TabButtions();
	void TabContent();
	void Accept();

	void ControllsCreate(JSON::SettingData& settingData);
	void WindowSizeCreate(JSON::SettingData& settingData);
private:
	bool _isSettings;
	std::vector<JSON::SettingData> _SettingsData;
	map<string, string>LoadedTextMap;
	//Settings
	Immage_Widget SettingsBackground;
	PageSlider_Widget< Colour, Colour> SettingsScrollBar;
	DropDown_Widget<std::string, std::string, std::string> SettingsDropdowns[10];
	DataSlider_Widget<std::string, std::string> SettingsSliders[10];
	Buttion_Widget<std::string> SettingsButtions[10];
	Input_Widget<std::string> ControllInput[20];
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

	//settings counts
	UINT SettingsDropCount = 0;
	UINT SettingSliderCount = 0;
	UINT SettingsButtionCount = 0;
	UINT SettingsInputCount = 0;

	//current tab open
	Tabs CurrentTab = GenralTab;

	bool LoadFlag = true;
	bool mouseLoad = true;
	float currentPY;
	float currentY;
	XMFLOAT2 curretJsonWinsize = { 0,0 };
	//for checkig whether setting is on screen;
	XMFLOAT2 boxPos;
	XMFLOAT2 boxSize;
};

