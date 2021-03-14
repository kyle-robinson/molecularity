#include "stdafx.h"
#include "ColourBlock.h"

ColourBlock::ColourBlock()
{
}

ColourBlock::ColourBlock(Colour colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, float AFactor)
{
	Function(colour, size, pos, AFactor);
}

ColourBlock::~ColourBlock()
{
}

bool ColourBlock::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	_ColourSprite.Initialize(Device, Contex, _Size.x, _Size.y, _Colour, cb_vs_matrix_2d);

	return true;
}


void ColourBlock::Draw(ID3D11DeviceContext* Contex,
	ID3D11Device* Device,ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix)
{  
	_ColourSprite.UpdateTex(Device, _Colour);
    _ColourSprite.SetInitialPosition(_Pos.x, _Pos.y, 0);
	_ColourSprite.SetScale(_Size.x, _Size.y);

	cb_ps_scene.data.alphaFactor = _AlfaFactor;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u,cb_ps_scene.GetAddressOf());
	_ColourSprite.Draw(WorldOrthoMatrix);

}

bool ColourBlock::Function(Colour colour, DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, float AFactor)
{
	_Colour = colour;
    _Size = size;
    _Pos = pos;
    _AlfaFactor = AFactor;
	
    return true;
}
