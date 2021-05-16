#include "stdafx.h"
#include "Tutorial_UI.h"

Tutorial_UI::Tutorial_UI()
{
}

Tutorial_UI::~Tutorial_UI()
{
}

void Tutorial_UI::Inizalize(ID3D11Device* device, ID3D11DeviceContext* contex, ConstantBuffer<CB_VS_matrix_2D>* cb_vs_matrix_2d)
{
}

void Tutorial_UI::Update()
{
	//tool instructions 
	{
		//tool controlls 

		//tool 1


		//too 2
	}

	//movement instructions
	{
		//movement


	}

	//other controlls 
	{
		//puase
	}


	//Game instruction
	{
		//get to exsit
		//puzzle
	}




}

void Tutorial_UI::BeginDraw(VertexShader& vert, PixelShader& pix, XMMATRIX WorldOrthMatrix, ConstantBuffer<CB_PS_scene>* _cb_ps_scene)
{
}

void Tutorial_UI::HandleEvent(Event* event)
{
	//need controlls 
	
}
