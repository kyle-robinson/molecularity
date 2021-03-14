#include "stdafx.h"
#include "DIMGUI_UI.h"
#include"Graphics/Graphics.h"
DIMGUI_UI::DIMGUI_UI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    SetBlackGoldStyle();
}

DIMGUI_UI::~DIMGUI_UI()
{
	ImGui::DestroyContext();
}

void DIMGUI_UI::Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context) const noexcept
{
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, context);
}

void DIMGUI_UI::BeginRender() const noexcept
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DIMGUI_UI::EndRender() const noexcept
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}



void DIMGUI_UI::SpwanDebug(Graphics& gfx)
{
    SpwanInstructionWindow();
    SpwanGrapicsWindow(gfx);
    gfx.spotLight.SpawnControlWindow();
    gfx.directionalLight.SpawnControlWindow();
    gfx.pointLight.SpawnControlWindow();
    gfx.spotLight.SpawnControlWindow();
   
  
}


void DIMGUI_UI::SetBlackGoldStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    style->FramePadding = ImVec2(4, 2);
    style->ItemSpacing = ImVec2(10, 2);
    style->IndentSpacing = 12;
    style->ScrollbarSize = 10;

    style->WindowRounding = 4;
    style->FrameRounding = 4;
    style->PopupRounding = 4;
    style->ScrollbarRounding = 6;
    style->GrabRounding = 4;
    style->TabRounding = 4;

    style->WindowTitleAlign = ImVec2(1.0f, 0.5f);
    style->WindowMenuButtonPosition = ImGuiDir_Right;

    style->DisplaySafeAreaPadding = ImVec2(4, 4);


    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    TitleFont = io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\NotoSerif-Black.ttf", 35);
}

void DIMGUI_UI::SpwanInstructionWindow()
{

    if (ImGui::Begin("Scene Instructions", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        if (ImGui::CollapsingHeader("Camera Controls", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("W                 Forward");
            ImGui::Text("A                 Left");
            ImGui::Text("S                 Backward");
            ImGui::Text("D                 Right");
            ImGui::Separator();
            ImGui::Text("F1                Default Camera");
            ImGui::Text("F2                Static Camera");
            ImGui::Text("F3                Debug Camera");
        }
        if (ImGui::CollapsingHeader("Multi-Tool Controls", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("1                 CONVERT Mode");
            ImGui::Text("Scroll Up/Down    Change Texture To Set");
            ImGui::Text("LMB On Cube       Set Selected Texture");
            ImGui::Separator();
            ImGui::Text("2                 RESIZE Mode");
            ImGui::Text("Scroll Up/Down    Change Size To Set");
            ImGui::Text("LMB On Cube       Set Selected Size");
            ImGui::Separator();
            ImGui::Text("E                 Pick-Up Cube");
        }
        if (ImGui::CollapsingHeader("Miscellaneous Controls"))
        {
            ImGui::Text("ESCAPE            Close Game");
        }
        if (ImGui::CollapsingHeader("Debug Controls", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Hold RMB          Rotate Camera");
            ImGui::Text("HOME              Enable Mouse");
            ImGui::Text("END               Disable Mouse");
            ImGui::Text("SPACE             Up");
            ImGui::Text("CTRL              Down");
        }
    }
    ImGui::End();
}

void DIMGUI_UI::SpwanGrapicsWindow(Graphics& gfx)
{
    if (ImGui::Begin("Graphics Controls", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        // update render target
        bool multiViewBool = static_cast<bool>(gfx.multiView);
        ImGui::Text("Enable Multi-View: ");
        ImGui::SameLine();
        ImGui::Checkbox("", &multiViewBool);
        gfx.multiView = static_cast<BOOL>(multiViewBool);

        // update texture usage
        ImGui::Text("Texture Usage: ");
        ImGui::SameLine();
        static int textureGroup = 0;
        if (ImGui::RadioButton("Apply", &textureGroup, 0))
            gfx.useTexture = TRUE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Discard", &textureGroup, 1))
            gfx.useTexture = FALSE;

        // update rasterizer
        ImGui::Text("Rasterizer: ");
        ImGui::SameLine();
        static int rasterizerGroup = 0;
        if (ImGui::RadioButton("Solid", &rasterizerGroup, 0))
            gfx.rasterizerSolid = true;
        ImGui::SameLine();
        if (ImGui::RadioButton("Wireframe", &rasterizerGroup, 1))
            gfx.rasterizerSolid = false;

        // update sampler
        static int activeSampler = 0;
        static bool selectedSampler[3];
        static std::string previewValueSampler = "Anisotropic";
        static const char* samplerList[]{ "Anisotropic", "Bilinear", "Point Sampling" };
        if (ImGui::BeginCombo("Sampler", previewValueSampler.c_str()))
        {
            for (unsigned int i = 0; i < IM_ARRAYSIZE(samplerList); i++)
            {
                const bool isSelected = i == activeSampler;
                if (ImGui::Selectable(samplerList[i], isSelected))
                {
                    activeSampler = i;
                    previewValueSampler = samplerList[i];
                }
            }

            switch (activeSampler)
            {
            case 0: gfx.samplerToUse = "Anisotropic"; break;
            case 1: gfx.samplerToUse = "Bilinear"; break;
            case 2: gfx.samplerToUse = "Point"; break;
            }

            ImGui::EndCombo();
        }

        // update blending
        ImGui::SliderFloat("Alpha", &gfx.alphaFactor, 0.0f, 1.0f, "%.1f");
        
    }
    ImGui::End();
}
