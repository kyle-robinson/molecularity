#pragma once
#ifndef TUTORIAL_UI_H
#define TUTORIAL_UI_H

#include "UI.h"
#include "Timer.h"
#include "Tool_Class.h"

enum class TutorialState
{
    GameTut,
    MovementTut,
    ToolTut,
    ToolTut2,
    OtherTut
};

/// <summary>
/// User interface for the tutorial part of level 1.
/// </summary>
class Tutorial_UI : public UI
{
public:
    Tutorial_UI();
    ~Tutorial_UI();

    void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts );
    void Update( float dt );
    void BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene );

    void TextLoad();
    void HandleEvent( Event* event );
private:
    void AddtoEvent();
    void RemoveFromEvent();

    // Text functions
    void GeneralTutorialText();
    void MovementTutorialText();
    void ToolTutorialText();
    void OtherTutorialText();

    // Move next function
    void MoveToNextPanel();

    void LoadKeyBinds( std::vector<JSON::SettingData> SettingsData );
    void SetTextPos( TextToDraw& text );
private:
    Timer timer;
    bool IsDraw = true;
    ColourBlock TextBackground;
    Tool_Class* Mode = nullptr;
    std::string name = "Tutorial";

    int stateNo;
    TutorialState CurrentState;
    std::vector<TextToDraw> _TextList;
    std::unordered_map<std::string, std::string> KeyBinds;
    std::unordered_map<std::string, std::string> LoadedTextMap;

    float xpos;
    float yPos;
    float textSizeY;
    float textSizeX;
};

#endif