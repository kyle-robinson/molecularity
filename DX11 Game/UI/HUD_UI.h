#pragma once
#ifndef HUD_UI_H
#define HUD_UI_H

#include "UI.h"
#include "Tool_Class.h"

/// <summary>
/// Handle the main user interface for each level in the game.
/// </summary>
class HUD_UI : public UI
{
public:
	HUD_UI();
	~HUD_UI();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts );
	void Update( float dt );
	void BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );
	void TextLoad();
	void HandleEvent( Event* event );
private:
	void AddtoEvent();
	void RemoveFromEvent();

	void CreateToolHud();
	void UpdateKeytext();
	void UpdateSettingsData( std::vector<JSON::SettingData>& SettingsData );
private:
	// HUD
	std::string LastKey;
	std::string key;
	bool canHoldCube = false;
	bool isDissCube = true;
	float hudScale = 1;
	bool isHudActive = true;
	TextToDraw PickupText;
	Image_Widget HUDImages[3];
	ColourBlock HudBakgrounds[2];
	Energy_Bar_Widget<Colour, Colour, std::string> HUDenergyWidget;

	Tool_Class* Mode = nullptr;
	std::unordered_map<std::string, std::string>LoadedTextMap;
};

#endif