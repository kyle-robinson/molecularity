#pragma once
#include "UI.h"
#include "Timer.h"
#include<Tool_Class.h>
using namespace std;

enum class TutorialState {
    GameTut,
    MovementTut,
    ToolTut,
    ToolTut2,
    OtherTut
};



class Tutorial_UI :
    public UI
{
public:
    Tutorial_UI();
     ~Tutorial_UI();
     void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d);
     void Update(float dt);
     void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

     void HandleEvent(Event* event);

private:
    string ConvertFromUnsignedCharTostring(unsigned char input);
private:

    string name = "Tutorial";


    bool IsDraw=true;
    Tool_Class* Mode = nullptr;
    Immage_Widget OutLines[2];

    Timer timer;

    ColourBlock TextBackground;

    TutorialState CurrentState;
    int stateNo;
    vector<TextToDraw> _TextList;
    map<string, string> KeyBindes;
    std::shared_ptr<TextRenderer> _TextRenderer;
};

