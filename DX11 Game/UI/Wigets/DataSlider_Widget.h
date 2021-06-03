#pragma once
#ifndef DATASLIDER_WIDGET_H
#define DATASLIDER_WIDGET_H

#include "widget.h"

template<typename BarTex, typename SliderTex>
class DataSlider_Widget :
	public widget
{
public:
	DataSlider_Widget() {}
	DataSlider_Widget( DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData );

	bool INITSprite( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d );
	void Draw( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix );
	void Function( DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData );

	uint32_t getData() { return DataOut; }

private:
	Sprite Bar;
	BarTex _BarColour;
	Sprite Slider;
	SliderTex _SliderColour;

	float PX = 0;
	uint32_t DataOut;
};

template<typename BarTex, typename SliderTex>
inline DataSlider_Widget<BarTex, SliderTex>::DataSlider_Widget( DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData )
{
	Function( size, pos, start, bar, slider, MData );
}

template<typename BarTex, typename SliderTex>
inline bool DataSlider_Widget<BarTex, SliderTex>::INITSprite( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d )
{
	Bar.Initialize( Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d );
	Slider.Initialize( Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d );
	return true;
}

template<typename BarTex, typename SliderTex>
inline void DataSlider_Widget<BarTex, SliderTex>::Draw( ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix )
{
	Bar.SetInitialPosition( _Pos.x, _Pos.y, 0 );
	Bar.SetScale( _Size.x, _Size.y );
	Bar.UpdateTex( Device, _BarColour );
	cb_ps_scene.data.alphaFactor = _AlphaFactor;
	cb_ps_scene.data.useTexture = true;

	if ( !cb_ps_scene.ApplyChanges() ) return;
	Contex->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
	Bar.Draw( WorldOrthoMatrix );

	Slider.UpdateTex( Device, _SliderColour );
	Slider.SetInitialPosition( ( _Pos.x + PX ) - 25 / 2, _Pos.y + ( 30 - ( 30 / 0.75 ) ) / 2, 0 );
	Slider.SetScale( 25, _Size.y / 0.75 );

	cb_ps_scene.data.alphaFactor = _AlphaFactor;
	cb_ps_scene.data.useTexture = true;

	if ( !cb_ps_scene.ApplyChanges() ) return;
	Contex->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
	Slider.Draw( WorldOrthoMatrix );
}

template<typename BarTex, typename SliderTex>
inline void DataSlider_Widget<BarTex, SliderTex>::Function( DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData )
{
	_Size = size;
	_Pos = pos;
	_BarColour = bar;
	_SliderColour = slider;
	PX = ( ( float )start / 100 ) * size.x;

	if (
		MData.Pos.x >= pos.x &&
		MData.Pos.x <= ( pos.x + size.x + 1 ) &&
		MData.Pos.y >= pos.y &&
		MData.Pos.y <= ( pos.y + size.y ) ) {
		if ( MData.LPress ) {
			PX = MData.Pos.x - pos.x;

		}
	}

	// return data
	DataOut = ( PX / size.x ) * 100;
}

#endif