#include "stdafx.h"
#include "Fonts.h"

Fonts::Fonts()
{}

Fonts::~Fonts()
{
	RemoveAll();
}

void Fonts::Initialize(ID3D11Device* device, ID3D11DeviceContext* contex)
{
	_Device = device;
	_Contex = contex;
}

map<string, std::shared_ptr<TextRenderer>> Fonts::GetFontList()
{
	return FontList;
}

std::shared_ptr<TextRenderer> Fonts::GetFont(string font)
{
	return FontList[font];
}

void Fonts::AddFont(string name, string file)
{
	//check if it is in list
	bool ToAdd = true;
	for (auto const& Font : FontList)
	{
		if (Font.first == name) {
			ToAdd = false;
			break;
		}
	}
	if (ToAdd) {
		FontList[name] = make_shared<TextRenderer>(file, _Device.Get(), _Contex.Get());
		CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, WinSize.x, WinSize.y);
		FontList[name]->UpdateViewPort(newViewport);
	}
	
}

void Fonts::AddFont(string name, string file, ID3D11Device* device, ID3D11DeviceContext* contex)
{

	//check if it is in list
	bool ToAdd = true;
	for (auto const& Font : FontList)
	{
		if (Font.first == name) {
			ToAdd = false;
			break;
		}
	}
	if (ToAdd) {
		FontList[name] = make_shared<TextRenderer>(file, device, contex);
		CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, WinSize.x, WinSize.y);
		FontList[name]->UpdateViewPort(newViewport);
	}

	
}

void Fonts::RemoveFont(string name)
{
	for (auto i = FontList.begin(); i != FontList.end(); i++) {
		if (i->first == name) {
			i = FontList.erase(i);
			break;
		}
	}
}

void Fonts::RemoveAll()
{
	FontList.clear();
}

void Fonts::ResizeViewAll(XMFLOAT2 Size)
{
	WinSize = Size;
	CD3D11_VIEWPORT newViewport = CD3D11_VIEWPORT(0.0f, 0.0f, Size.x, Size.y);
	for (auto const& Font : FontList) {
		Font.second->UpdateViewPort(newViewport);
	}

}
