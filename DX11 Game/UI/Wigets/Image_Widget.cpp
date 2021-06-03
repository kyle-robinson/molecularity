#include "stdafx.h"
#include "Image_Widget.h"

Image_Widget::Image_Widget()
{
	
}

Image_Widget::~Image_Widget()
{
}

bool Image_Widget::Function(std::string texture, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos)
{

	string TexFile = "Resources\\Textures\\";
	TexFile.append(texture);


	if (_TexFile != TexFile) {
		_TexFile = TexFile;
		updateText = true;
	}
	_Size = size;
	_Pos = pos;
	_AlphaFactor = 1.0f;
	
	return false;
}

bool Image_Widget::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	updateText = true;
	_Image.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
	return true;
}

void Image_Widget::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix)
{
	_Image.SetInitialPosition(_Pos.x, _Pos.y, 0);
	_Image.SetScale(_Size.x, _Size.y);

	if (updateText) {
		_Image.UpdateTex(Device, _TexFile);
		updateText = false;
	}

	cb_ps_scene.data.useTexture = true;
	cb_ps_scene.data.alphaFactor = _AlphaFactor;


	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());

	_Image.Draw(WorldOrthoMatrix);
}