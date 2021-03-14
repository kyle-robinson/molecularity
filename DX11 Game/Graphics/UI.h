#pragma once


#include "Graphics/Custom_UI.h"
#include<map>;

class Input;
class Graphics;

//ui driver code
class UI
{
public:
	UI();
	

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context);
	~UI();


	void Draw(Graphics* level,VertexShader& vert, PixelShader& pix);
	
	
	//turn off ui
	bool getToDraw()const { return IsToDraw; }
	void setToDraw(bool ToDraw) { IsToDraw = ToDraw; }


	std::shared_ptr <Custom_UI> getCustomUi(){
		return UiList["All"];
	}
private:

private:

	


	//draw all
	bool IsToDraw = true;

	std::map<string, std::shared_ptr <Custom_UI>> UiList;


};

