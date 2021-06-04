#pragma once
#ifndef PAGESLIDER_WIDGET_H
#define PAGESLIDER_WIDGET_H

#include "widget.h"

template<typename BarTex,typename SliderTex>
class PageSlider_Widget : public widget
{
public:
    PageSlider_Widget(){}
    PageSlider_Widget(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData);

	bool INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d);
    void Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix);
    void Function(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData);
    
	float getPagePos() { return PagePos; }
	float getPY() { return PY; }
	void setPY(float py) { PY = py; }
	void SetPageSize(float pageSize) { PageSize = pageSize; }
private:
	Sprite Bar;
    BarTex _BarColour;
	Sprite Slider;
    SliderTex _SliderColour;

	float PY = 0;
	float PagePos = 0;
	float PageSize;
};

template<typename BarTex, typename SliderTex>
inline PageSlider_Widget<BarTex, SliderTex>::PageSlider_Widget(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData)
{
	Function( size,  pos,  start, bar,  slider,  MData);
}

template<typename BarTex, typename SliderTex>
inline bool PageSlider_Widget<BarTex, SliderTex>::INITSprite(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{	
	Bar.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
	Slider.Initialize(Device, Contex, _Size.x, _Size.y, "", cb_vs_matrix_2d);
	return true;
}

template<typename BarTex, typename SliderTex>
inline void PageSlider_Widget<BarTex, SliderTex>::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix)
{
	Bar.UpdateTex(Device, _BarColour);
	Bar.SetScale(_Size.x, _Size.y + 100);
	Bar.SetInitialPosition(_Pos.x, _Pos.y, 0);

	cb_ps_scene.data.alphaFactor = 0.8;
	cb_ps_scene.data.useTexture = false;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	Bar.Draw(WorldOrthoMatrix);

	Slider.UpdateTex(Device, _SliderColour);
	Slider.SetInitialPosition(_Pos.x, _Pos.y + PY, 0);
	Slider.SetScale(_Size.x, 100);

	cb_ps_scene.data.alphaFactor = _AlphaFactor;
	cb_ps_scene.data.useTexture = false;
	
	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	Slider.Draw(WorldOrthoMatrix);
}

template<typename BarTex, typename SliderTex>
inline void PageSlider_Widget<BarTex, SliderTex>::Function(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 pos, int start, BarTex bar, SliderTex slider, MouseData MData)
{
	_Size = size;
	
	_Pos = pos;
	_BarColour = bar;
	_SliderColour = slider;

	if (
		MData.Pos.x >= pos.x &&
		MData.Pos.x <= (pos.x + size.x + 1) &&
		MData.Pos.y >= pos.y &&
		MData.Pos.y <= (pos.y + size.y)) {
		if (MData.LPress) {
			PY = MData.Pos.y- (pos.y);

		}
	}

	//return data
	PagePos= PageSize *(PY / size.y);;
}

#endif