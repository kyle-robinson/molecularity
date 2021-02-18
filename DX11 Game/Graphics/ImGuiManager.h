#pragma once
#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H

/// <summary>
/// The main manager class for setting up ImGui components.
/// Contains a function to control the main graphical components of a scene from GraphicsContainer.h
/// Is a 'friend' of GraphicsContainer.h allowing to access its private and protected member variables/functions.
/// </summary>

class GraphicsContainer;
class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();
	void Initialize( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context ) const noexcept;
	void BeginRender() const noexcept;
	void EndRender() const noexcept;
	void SpawnInstructionWindow() const noexcept;
	void SpawnGraphicsWindow( GraphicsContainer& gfx ) const noexcept;
private:
	void SetBlackGoldStyle();
};

#endif