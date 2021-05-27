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
	void TabButtons();
	void TabContent();
	void Accept();

	void ControlsCreate(JSON::SettingData& settingData);
	void WindowSizeCreate(JSON::SettingData& settingData);
private:
	bool _isSettings;
	std::vector<JSON::SettingData> _SettingsData;
	map<string, string>LoadedTextMap;
	//Settings
	Image_Widget SettingsBackground;
	PageSlider_Widget< Colour, Colour> SettingsScrollBar;
	DropDown_Widget<std::string, std::string, std::string> SettingsDropdowns[10];
	DataSlider_Widget<std::string, std::string> SettingsSliders[10];
	Button_Widget<std::string> SettingsButtons[10];
	Input_Widget<std::string> ControlInput[20];
	vector<TextToDraw> PauseTextTitles;
	vector<TextToDraw> PauseTextPG;


	vector<string> ButtonTexDrop = { "Resources\\Textures\\Settings\\DropArrow_Blue.dds",
		"Resources\\Textures\\Settings\\DropArrow_Blue.dds",
		"Resources\\Textures\\Settings\\DropArrow.dds" };

	vector<string> ButtonBackDrop = { "Resources\\Textures\\Settings\\Input_Blue.dds",
		"Resources\\Textures\\Settings\\Input_Blue.dds",
		"Resources\\Textures\\Settings\\Input_Yellow.dds" };



	vector<string> ButtonTex = { "Resources\\Textures\\Settings\\Button_SettingsBar_Hover.dds",
		"Resources\\Textures\\Settings\\Button_SettingsBar_Hover.dds",
		"Resources\\Textures\\Settings\\Button_SettingsBar_Up.dds" };

	vector<string> AcceptButton = { "Resources\\Textures\\UI_Buttons\\Button_1_Down.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds" };

	XMFLOAT2 TabTextPos;

	//settings counts
	UINT SettingsDropCount = 0;
	UINT SettingSliderCount = 0;
	UINT SettingsButtonCount = 0;
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

