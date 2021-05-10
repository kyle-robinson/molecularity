#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include"Graphics/TextRenderer.h"
#include "Sprite.h"

struct MouseData
{
	XMFLOAT2 Pos;
	bool LPress;
	bool RPress;
	bool MPress;
};

using namespace std;
class widget
{
public:
	// functions
	widget();
	~widget();

	virtual void Function();
	virtual void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix);

	// get/sets
	void setSize(XMFLOAT2 size) { _Size = size; }
	XMFLOAT2 GetSize() { return _Size; }

	void setPos(XMFLOAT2 pos) { _Pos = pos; }
	XMFLOAT2 GetPos() { return _Pos; }

	void SetAlfaFactor(float AF) { _AlphaFactor = AF; }
	float GetAlfaFactor() { return _AlphaFactor; }

protected:
	// vars
	XMFLOAT2 _Size;
	XMFLOAT2 _Pos;
	float _AlphaFactor = 1.0f;
};

#endif