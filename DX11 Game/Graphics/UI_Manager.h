#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

class Input;
#include "UI.h"
#include "Fonts.h"
#include "EventSystem/Listener.h"

/// <summary>
/// Manager class for each of the UI components.
/// </summary>
class UI_Manager : public Listener
{
public:
	UI_Manager();
	~UI_Manager();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d );


	void Update( float dt );
	void Draw( VertexShader& vert, PixelShader& pix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );

	std::shared_ptr <UI> GetCustomUi( std::string UIName );

	void AddUi( std::shared_ptr < UI> NewUI, std::string Name );
	void RemoveUI( std::string Name );
	void RemoveAllUI();


	// Hiding/Showing UI
	void HideAllUI();
	void ShowAllUi();
	void ShowUi( std::string Name );
	void HideUi( std::string Name );

	void HandleEvent( Event* event );
private:
	void AddtoEvent();
	void RemoveFromEvent();
private:
	XMFLOAT4X4 WorldOrthMatrix;

	std::unordered_map<std::string, std::shared_ptr<UI>> UiList;
	std::vector<std::string> UiToDraw;
	XMFLOAT2 WinSize;

	// Fonts
	std::shared_ptr<Fonts> FontsList;
};

#endif