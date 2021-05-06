#pragma once
#include<queue>
#include <Utility/JSON_Helper.h>
#include<UI/WidgetIncludes.h>
#include"TextRenderer.h"
#include<Utility/EventSystem/Listener.h>
struct TextToDraw {
	string _Text;
	XMFLOAT2 _Position;
	XMVECTORF32 _Colour;
};

class UI:public Listener
{
public:
	UI() {};

	virtual void Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)=0;
	virtual void Update()=0;
	virtual void BeginDraw(VertexShader& vert, PixelShader& pix,XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)=0;
	virtual void HandleEvent(Event* event)=0;

	void SetCb_ps_scene(ConstantBuffer<CB_PS_scene>* cb_ps_scene) { _cb_ps_scene = cb_ps_scene; }
	void SetCb_vs_matrix_2d(ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d) {_cb_vs_matrix_2d = cb_vs_matrix_2d;}
protected:

	//Grapics Infromation
	Microsoft::WRL::ComPtr <ID3D11Device>_Device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> _Contex;
	ConstantBuffer<CB_PS_scene>* _cb_ps_scene;
	ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d;
	XMFLOAT2 _SizeOfScreen;

	//inputs
	MouseData _MouseData;
	unsigned char Key;
};

