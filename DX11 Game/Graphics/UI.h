#pragma once


#include "Graphics/Custom_UI.h"


class Input;
class Graphics;

//ui driver code
class UI
{
public:
	UI();
	

	void Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context, Graphics* level);
	~UI();


	void Draw(Graphics* level);
	
	
	//turn off ui
	bool getToDraw()const { return IsToDraw; }
	void setToDraw(bool ToDraw) { IsToDraw = ToDraw; }

	shared_ptr < Custom_UI> getCustomUi() { return CustomUiManger; }

private:

private:

	//Grapics Infromation
	//ID3D11Device*Device;
	//ID3D11DeviceContext* Contex;
	//ConstantBuffer<CB_PS_scene> _cb_ps_scene;
	//ConstantBuffer<CB_VS_matrix_2D> _cb_vs_matrix_2d;
	//XMFLOAT2 _SizeOfScreen;


	//draw all
	bool IsToDraw = true;

	shared_ptr <Custom_UI> CustomUiManger;


};

