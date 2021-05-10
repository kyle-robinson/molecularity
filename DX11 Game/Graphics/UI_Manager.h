#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <EventSystem/Listener.h>
#include <UI/UI.h>
#include <map>

class Input;
class Graphics;

// ui driver code
class UI_Manager : public Listener
{
public:
	UI_Manager();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d );
	~UI_Manager();

	void Update();
	void Draw( VertexShader& vert, PixelShader& pix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );

	// turn off ui
	bool GetToDraw()const { return IsToDraw; }
	void SetToDraw( bool ToDraw ) { IsToDraw = ToDraw; }

	std::shared_ptr <UI> GetCustomUi( string UIName ) { return UiList[UIName]; }
	void AddUi( std::shared_ptr<UI> NewUI, string Name );
	void HandleEvent( Event* event );

private:
	//draw all
	bool IsToDraw = true;
	XMFLOAT4X4 WorldOrthMatrix;
	std::map<std::string, std::shared_ptr<UI>> UiList;
};

#endif