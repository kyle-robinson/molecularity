#pragma once
#include<EventSystem/Listener.h>
#include <UI/UI.h>
#include<map>
#include <UI/Fonts.h>
class Input;

//ui driver code
class UI_Manager : public Listener
{
public:
	UI_Manager();
	~UI_Manager();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d);
	

	void Update(float dt);
	void Draw(VertexShader& vert, PixelShader& pix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene);

	std::shared_ptr <UI> GetCustomUi(std::string UIName);

	void AddUi(std::shared_ptr < UI> NewUI, std::string Name);
	void RemoveUI(std::string Name);
	void RemoveAllUI();


	//for hideing and showing ui
	void HideAllUI();
	void ShowAllUi();
	void ShowUi(string Name);
	void HideUi(string Name);

	//event system
	void HandleEvent(Event* event);
private:
	void AddtoEvent();
	void RemoveFromEvent();
private:

	XMFLOAT4X4 WorldOrthMatrix;

	std::map<std::string, std::shared_ptr <UI>> UiList;
	vector<string>UiToDraw;
	XMFLOAT2 WinSize;


	//Fonts
	std::shared_ptr<Fonts> FontsList;
};

