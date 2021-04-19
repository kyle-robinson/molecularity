#pragma once
#include "Graphics/Custom_UI.h"
#include<map>;

class Input;
class Graphics;
//ui driver code
class UI_Manager
{
public:
	UI_Manager();
	

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context);
	~UI_Manager();


	void Draw(Graphics* level,VertexShader& vert, PixelShader& pix);
	
	
	//turn off ui
	bool GetToDraw()const { return IsToDraw; }
	void SetToDraw(bool ToDraw) { IsToDraw = ToDraw; }

	std::shared_ptr <Custom_UI> GetCustomUi(string UIName){
		return UiList["All"];
	}


private:

private:
	//draw all
	bool IsToDraw = true;

	bool GetIsPaused() { return isPaused; }
	void SetIsPuased(bool pause) { isPaused = pause; }

	bool isPaused = false;
	bool isSettings = false;

	std::map<string, std::shared_ptr <Custom_UI>> UiList;

};

