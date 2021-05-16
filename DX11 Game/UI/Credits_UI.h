#pragma once
#include "UI.h"
using namespace std;
class Credits_UI :
    public UI
{
public:
	Credits_UI();
	~Credits_UI();


	void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
	void Update(float dt);
	void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	void HandleEvent(Event* event);
	 
private:

	Immage_Widget Images[3];
	ColourBlock Background;
	vector<TextToDraw> _TextList;

	std::shared_ptr<TextRenderer>  _TextRenderer;
	
	vector < pair < string, string > > CreditsINFO;
	UINT32 LevelTo = 2;
	int minus = 0;
	float YPos;
};

