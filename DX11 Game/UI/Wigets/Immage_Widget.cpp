#include "stdafx.h"
#include "Immage_Widget.h"

Immage_Widget::Immage_Widget()
{
}

Immage_Widget::Immage_Widget(string texture, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos)
{
	Function(texture, size, pos);
}

Immage_Widget::~Immage_Widget()
{
}

bool Immage_Widget::Function(string texture, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos)
{
	string TexFile = "Resources\\Textures\\";
	TexFile.append(texture);
	_TexFile = TexFile;
	_Size = size;
	_Pos = pos;
	_AlfaFactor = 1.0f;
	
	return false;
}

bool Immage_Widget::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	_Immage.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
	return true;
}

void Immage_Widget::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix)
{
	
	_Immage.SetInitialPosition(_Pos.x, _Pos.y, 0);
	_Immage.SetScale(_Size.x, _Size.y);
	_Immage.UpdateTex(Device, _TexFile);

	cb_ps_scene.data.useTexture = true;
	cb_ps_scene.data.alphaFactor = _AlfaFactor;


	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());

	_Immage.Draw(WorldOrthoMatrix);

}
