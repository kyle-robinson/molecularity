#pragma once
#include "UI.h"
using namespace std;
class Tutorial_UI :
    public UI
{
public:
    Tutorial_UI();
     ~Tutorial_UI();
     void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
     void Update();
     void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

     void HandleEvent(Event* event);

private:


    Immage_Widget Images[3];
    ColourBlock Bakgrounds[2];
    vector<TextToDraw> Text;

    std::shared_ptr<TextRenderer> TextRenderer;
};

