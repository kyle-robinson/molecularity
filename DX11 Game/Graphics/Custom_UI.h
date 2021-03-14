#pragma once

#include<queue>
#include <Utility/JSON_Helper.h>
#include<UI/WidgetIncludes.h>
#include"TextRenderer.h"

using namespace DirectX;
using namespace std;


enum TabStae {
	Active,
	NotActive
};

typedef enum Tabs {
	GrapicsTab,
	GenralTab,
	SoundTab,
	ControlsTab
};

struct ToDraw
{
	string Name;
	XMFLOAT2 pos;
	XMFLOAT2 size;
	Colour colour;
	string texFile;

};

struct textToDraw {
	string text;
	XMFLOAT2 position;
	XMVECTORF32 colour;
};



class Grapics;
class Custom_UI
{

public:
	
	Custom_UI();
	
	~Custom_UI();

	void Inizalize(Graphics* gfx);

	//Draw
	void BeginDraw(Graphics* gfx);

	//PreSet UI
	void MainMenu(Graphics* gfx);
	void GameHUD(Graphics* gfx);
	void Settings(Graphics* gfx);
	void Pause(Graphics* gfx);
	void PreMenuItems(Graphics* gfx);

	//input
	void KeyInput(unsigned char key) {
		 Key = key;
	}
	void MouseInput(float x, float y,bool press) {
		_MouseData.Pos = { x,y };
		_MouseData.LPress = press;
	}

	bool isPaused = false;
	bool isSettings = false;

private:
	
	//pre load
	void LoadTextures();
	void INITWigets(Graphics* gfx);
	void INITTexRender(Graphics* gfx);
//vars
private:

	//Json Data
	string JsonFile;
	vector<JSON::SettingData> _SettingsData;


	//inputs
	MouseData _MouseData;
	unsigned char Key;

	//textures
	std::map<string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> Textures;

	//Set Up Widgets

	//HUD
	Immage_Widget HUDImages[2];
	Energy_Bar_Widget<Colour, Colour, string> HUDenergyWidget;
	Energy_Bar_Widget<Colour, Colour, string> HUDHealthWidget;
	vector<string> HUDText;
	TextRenderer HUDTextRenderer;

	//Puase
	Buttion_Widget PuaseButtions[4];
	ColourBlock PuaseBakgtound;
	vector<string> PuaseText;

	//Settings
	ColourBlock SettingsBakgtound;
	PageSlider_Widget< Colour, Colour> SettingsScrollBar;
	DropDown_Widget<string> SettingsDropdowns[10];
	DataSlider_Widget<Colour, Colour> SettingsSliders[10];
	Buttion_Widget SettingsButtions[10];
	Input_Widget ControllInput[20];
	vector<textToDraw> SettingsText;


	UINT SettingsDropCount = 0;
	UINT SettingSliderCount = 0;
	UINT SettingsButtionCount = 0;
	UINT SettingsInputCount = 0;

	Tabs CurrentTab= GenralTab;

	//MainMenu
	ColourBlock MainmenuBakgtound;
	Buttion_Widget MainMenuButtions[5];
	Immage_Widget Titlecard;

	//PreMenu
	ColourBlock PreBakgtound;
	Immage_Widget PremenuImmages[2];

	TextRenderer HeadderTextRenderer;
	TextRenderer PGTextRenderer;
	TextRenderer GameTextRenderer;
	
};


//TODO
//Creat Child Class
//main Menu
//Game UI - puase,HUD
//Pre Game


