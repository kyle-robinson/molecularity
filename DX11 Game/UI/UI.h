#pragma once
#ifndef UI_H
#define UI_H

#include "JSON_Helper.h"
#include "WidgetIncludes.h"
#include "TextRenderer.h"
#include "Fonts.h"
#include "EventSystem/Listener.h"

struct TextToDraw
{
	std::string _Text;
	XMFLOAT2 _Position;
	XMVECTORF32 _Colour;
};

class UI : public Listener
{
public:
	UI() {};
	~UI();

	virtual void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d, std::shared_ptr<Fonts> fonts );
	virtual void Update( float dt ) = 0;
	virtual void BeginDraw( VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene ) = 0;
	virtual void HandleEvent( Event* event ) = 0;
	virtual void TextLoad() = 0;
	void SetCb_ps_scene( ConstantBuffer<CB_PS_scene>* cb_ps_scene ) { _cb_ps_scene = cb_ps_scene; }
	void SetCb_vs_matrix_2d( ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d ) { _cb_vs_matrix_2d = cb_vs_matrix_2d; }

	void SetSizeOfScreen( XMFLOAT2 screensize ) { _SizeOfScreen = screensize; }

protected:
	std::string ConvertFromUnsignedCharToString( unsigned char input );

	// Graphics Info
	Microsoft::WRL::ComPtr <ID3D11Device>_Device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> _Contex;
	ConstantBuffer<CB_PS_scene>* _cb_ps_scene;
	ConstantBuffer<CB_VS_matrix_2D>* _cb_vs_matrix_2d;
	XMFLOAT2 _SizeOfScreen;

	// Fonts
	std::shared_ptr<Fonts> FontsList;

	// Inputs
	MouseData _MouseData;
	unsigned char Key;
};

#endif