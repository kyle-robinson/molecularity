#pragma once
#include"TextRenderer.h"

//Fronts From :https://fonts.google.com/specimen/Open+Sans#license 
//License: https://www.apache.org/licenses/LICENSE-2.0.html
using namespace std;
class Fonts
{
public:
	Fonts();
	~Fonts();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* contex);

	map<string, std::shared_ptr<TextRenderer>> GetFontList();
	std::shared_ptr<TextRenderer> GetFont(string font);


	//add fonts
	void AddFont(string name, string file);
	void AddFont(string name, string file,ID3D11Device* device, ID3D11DeviceContext* contex);

	//remove font
	void RemoveFont(string name);
	void RemoveAll();

	void ResizeViewAll(XMFLOAT2 Size);

private:
	map<string, std::shared_ptr<TextRenderer>> FontList;
	XMFLOAT2 WinSize;
	Microsoft::WRL::ComPtr <ID3D11Device>_Device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> _Contex;
};

