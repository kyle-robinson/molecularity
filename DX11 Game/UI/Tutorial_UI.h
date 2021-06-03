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


/// <summary>
/// Tutorial UI
/// </summary>
class Tutorial_UI :
    public UI
{
public:
    Tutorial_UI();
     ~Tutorial_UI();
     void Initialize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d,std::shared_ptr<Fonts> fonts);
     void Update(float dt);
     void BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);
     void TextLoad();
     void HandleEvent(Event* event);

     //functions
private:
    
    void AddtoEvent();
    void RemoveFromEvent();

    

    //Text functions
    void GenralTutorialText();
    void MovementTutorialText();
    void ToolTutorialText();
    void OtherTutorialText();

    //move next function
    void MoveToNextPannle();


    void LoadKeyBinds(std::vector<JSON::SettingData> SettingsData);
    void SetTextPos(TextToDraw& text);
private:

    string name = "Tutorial";


    bool IsDraw=true;
    Tool_Class* Mode = nullptr;
  
    Timer timer;

    ColourBlock TextBackground;

    TutorialState CurrentState;
    int stateNo;
    vector<TextToDraw> _TextList;
    unordered_map<string, string> KeyBinds;
    unordered_map<string, string> LoadedTextMap;

    float textSizeY;
    float textSizeX;
    float yPos;
    float xpos;
};

