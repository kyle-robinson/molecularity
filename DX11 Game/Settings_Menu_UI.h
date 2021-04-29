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
	bool _isSettings;
	std::vector<JSON::SettingData> _SettingsData;
	//Settings
	Immage_Widget SettingsBakgtound;
	PageSlider_Widget< Colour, Colour> SettingsScrollBar;
	DropDown_Widget<string> SettingsDropdowns[10];
	DataSlider_Widget<Colour, Colour> SettingsSliders[10];
	Buttion_Widget<Colour> SettingsButtions[10];
	Input_Widget ControllInput[20];
	vector<TextToDraw> PuaseTextTitles;
	vector<TextToDraw> PuaseTextPG;

	XMFLOAT2 TabTextPos;

	UINT SettingsDropCount = 0;
	UINT SettingSliderCount = 0;
	UINT SettingsButtionCount = 0;
	UINT SettingsInputCount = 0;

	Tabs CurrentTab = GenralTab;


	std::shared_ptr<TextRenderer>  HeadderTextRenderer;
	std::shared_ptr<TextRenderer>  PGTextRenderer;

};

