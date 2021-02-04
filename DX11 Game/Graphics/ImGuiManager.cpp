#include "stdafx.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}

void ImGuiManager::Initialize( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context ) const noexcept
{
    ImGui_ImplWin32_Init( hWnd );
    ImGui_ImplDX11_Init( device, context );
}

void ImGuiManager::BeginRender() const noexcept
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::EndRender() const noexcept
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
}

void ImGuiManager::SpawnDemoWindow()
{
    if ( ImGui::Begin( "Test Window", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
    {
        static float demo = 1.0f;
        ImGui::SliderFloat( "Test Slider", &demo, 0.0f, 1.0f );
    } ImGui::End();
}