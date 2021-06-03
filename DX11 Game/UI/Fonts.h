#pragma once
#ifndef FONTS_H
#define FONTS_H

#include"TextRenderer.h"

//Fonts From :https://fonts.google.com/specimen/Open+Sans#license 
//License: https://www.apache.org/licenses/LICENSE-2.0.html

/// <summary>
/// Handle the addition/removal and initialization of fonts in the game.
/// </summary>
class Fonts
{
public:
	Fonts();
	~Fonts();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* contex );

	std::shared_ptr<TextRenderer> GetFont( std::string font );
	std::unordered_map<std::string, std::shared_ptr<TextRenderer>> GetFontList();

	// Add fonts
	void AddFont( std::string name, std::string file );
	void AddFont( std::string name, std::string file, ID3D11Device* device, ID3D11DeviceContext* contex );

	// Remove font
	void RemoveFont( std::string name );
	void RemoveAll();

	void ResizeViewAll( XMFLOAT2 Size );

private:
	XMFLOAT2 WinSize;
	Microsoft::WRL::ComPtr <ID3D11Device>_Device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> _Contex;
	std::unordered_map<std::string, std::shared_ptr<TextRenderer>> FontList;
};

#endif