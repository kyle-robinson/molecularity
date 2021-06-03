#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include "TextRenderer.h"
#include "Sprite.h"
#include <string>

struct MouseData
{
	XMFLOAT2 Pos;
	bool LPress;
	bool RPress;
	bool MPress;
};

class Widget
{
public:
	Widget();
	~Widget();

	virtual void Function();
	virtual void Draw( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene,
		ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix );

	void SetSize( XMFLOAT2 size ) { _Size = size; }
	XMFLOAT2 GetSize() { return _Size; }

	void SetPos( XMFLOAT2 pos ) { _Pos = pos; }
	XMFLOAT2 GetPos() { return _Pos; }

	void SetAlphaFactor( float AF ) { _AlphaFactor = AF; }
	float GetAlphaFactor() { return _AlphaFactor; }
protected:
	XMFLOAT2 _Pos;
	XMFLOAT2 _Size;
	bool updateText = true;
	float _AlphaFactor = 1.0f;
};

#endif