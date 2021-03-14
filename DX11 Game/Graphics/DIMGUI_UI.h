#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

using namespace ImGui;
using namespace DirectX;
using namespace std;

class Graphics;

class DIMGUI_UI
{
public:
	//
	DIMGUI_UI();
	~DIMGUI_UI();

	void Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context) const noexcept;
	void BeginRender() const noexcept;
	void EndRender() const noexcept;

	
	//pre set

	void SpwanConsole();
	void SpwanDebug(Graphics& gfx);
	
private:

	//styles
	void SetBlackGoldStyle();
	void ButtionStyle();
	void TipStyle();
	void setFonts();

	//debug windows
	void SpwanInstructionWindow();
	void SpwanGrapicsWindow(Graphics& gfx);

public:

private:

	

	//fonts
	ImFont* TitleFont;
	ImFont* SubTitleFont;
	ImFont* PGFont;







};

