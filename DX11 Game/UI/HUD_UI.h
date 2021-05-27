#pragma once
#include "UI.h"
#include<Tool_Class.h>
using namespace std;
class HUD_UI:
    public UI
{
public:
	HUD_UI();
	~HUD_UI();

	 void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts);
	 void Update(float dt);
	 void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene) ;
	 void TextLoad();
	 void HandleEvent(Event* event);
private:

	void AddtoEvent();
	void RemoveFromEvent();


	void CreateToolHud();
	void UpdateKeytext();
	void UpdateSettingsData(std::vector<JSON::SettingData>& SettingsData);
private:
	//HUD
	string key;
	bool canHoldCube = false;
	float hudScale = 1;
	bool isHudActive=true;
	TextToDraw PickupText;
	Image_Widget HUDImages[3];
	ColourBlock HudBakgrounds[2];
	Energy_Bar_Widget<Colour, Colour, string> HUDenergyWidget;

	Tool_Class* Mode= nullptr;
	
	map<string, string>LoadedTextMap;
};

