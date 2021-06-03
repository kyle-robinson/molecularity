#include "stdafx.h"
#include "Widget.h"

Widget::Widget() { }

Widget::~Widget() { }

void Widget::Function()
{ }

void Widget::Draw( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene,
	ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix )
{ }