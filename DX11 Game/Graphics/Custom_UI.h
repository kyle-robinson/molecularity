#pragma once

#include<queue>
#include <Utility/JSON_Helper.h>
#include<UI/WidgetIncludes.h>
#include"TextRenderer.h"
#include<Utility/EventSystem/Listener.h>


#include<Objects/CubeProperties.h>

using namespace DirectX;
using namespace std;


typedef enum Tabs {
	GrapicsTab,
	GenralTab,
	SoundTab,
	ControlsTab
};

struct textToDraw {
	string text;
	XMFLOAT2 position;
	XMVECTORF32 colour;
};



class Grapics;
class Custom_UI: public Listener
{

public:
	
	Custom_UI();
	
	~Custom_UI();

	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex);

	//Draw
	void BeginDraw(Graphics* gfx, VertexShader& vert, PixelShader& pix);

	//PreSet UI
	void MainMenu(Graphics* gfx);
	void GameHUD(Graphics* gfx);
	void Settings();
	void Pause();
	void PreMenuItems(Graphics* gfx);

	//input
	void KeyInput(unsigned char key) {
		 Key = key;
	}
	void MouseInput(float x, float y,bool press) {
		_MouseData.Pos = { x,y };
		_MouseData.LPress = press;
	}

	void HandleEvent(Event* event);
	void setScreenSize(XMFLOAT2 size) { _SizeOfScreen = size; }

	void setcb_ps_scene(ConstantBuffer<CB_PS_scene>& cb_ps_scene) { _cb_ps_scene = cb_ps_scene; }
	void setcb_vs_matrix_2d(ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d) {
		_cb_vs_matrix_2d = cb_vs_matrix_2d;
		INITWigets();
	}

	bool GetIsPaused() { return isPaused; }
	void SetIsPuased(bool pause) { isPaused = pause; }

	bool isPaused = false;
	bool isSettings = false;

private:
	void CleanUp();
	//pre load
	void LoadTextures();
	void INITWigets();
	void INITTexRender();
//vars
private:

	//Json Data
	string JsonFile;
	std::vector<JSON::SettingData> _SettingsData;


	//inputs
	MouseData _MouseData;
	unsigned char Key;

	//textures
	std::map<string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> Textures;

	//Set Up Widgets

	//HUD
	float hudScale = 1;
	Immage_Widget HUDImages[3];
	ColourBlock HudBakgrounds[2];
	Energy_Bar_Widget<Colour, Colour, string> HUDenergyWidget;
	int energy = 100;
	ToolType ToolSetting;
	std::shared_ptr<TextRenderer>  HUDTextRenderer;

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

	std::shared_ptr<TextRenderer>  HeadderTextRenderer;
	std::shared_ptr<TextRenderer>  PGTextRenderer;
	std::shared_ptr<TextRenderer>  GameTextRenderer;
	


	//Grapics Infromation
	Microsoft::WRL::ComPtr < ID3D11Device>_Device;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext> _Contex;
	ConstantBuffer<CB_PS_scene> _cb_ps_scene;
	ConstantBuffer<CB_VS_matrix_2D> _cb_vs_matrix_2d;
	XMFLOAT2 _SizeOfScreen;
};


//TODO
//Creat Child Class
//main Menu
//Game UI - puase,HUD
//Pre Game


