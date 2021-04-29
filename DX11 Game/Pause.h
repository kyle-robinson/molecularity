#pragma once
#include "UI.h"
class Pause :
    public UI
{
public:
	Pause();
	~Pause();
	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	void Update();
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	bool GetPause() { return _isPuased; }

	void HandleEvent(Event* event);

private:
	void CleanUp();
private:

	bool _isPuased;
	//Puase
	Buttion_Widget<Colour> PuaseButtions[4];
	ColourBlock PuaseBakgtound;
	vector<TextToDraw> PuaseTextTitles;
	vector<TextToDraw> PuaseTextPG;
	//fonts
	std::shared_ptr<TextRenderer>  HeadderTextRenderer;
	std::shared_ptr<TextRenderer>  PGTextRenderer;

	
};

