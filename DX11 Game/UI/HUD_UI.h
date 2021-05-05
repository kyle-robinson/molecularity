#pragma once
#include "UI.h"
#include<Objects/CubeProperties.h>
class HUD_UI :
    public UI
{
public:
	HUD_UI();
	~HUD_UI();

	 void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	 void Update();
	 void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene) ;

	 void HandleEvent(Event* event);
private:
	//HUD
	float hudScale = 1;
	Immage_Widget HUDImages[3];
	ColourBlock HudBakgrounds[2];
	Energy_Bar_Widget<Colour, Colour, string> HUDenergyWidget;
	int energy = 100;
	CubeProperties* Mode= nullptr;
	std::shared_ptr<TextRenderer>  HUDTextRenderer;
};

