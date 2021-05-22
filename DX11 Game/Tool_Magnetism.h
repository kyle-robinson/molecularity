#pragma once
#include "Tool_Function.h"
class Tool_Magnetism :
    public Tool_Function
{
public:
	Tool_Magnetism();
	~Tool_Magnetism();

	void ChangeCurrent(int number)override;
	void ChangeCurrent(std::string name)override;
	void  AddOneToCurrent() override;
	void  MinusOneTOCurrent() override;

	MagnetismMode GetMagMode() { return CurrentMode; }
	ToolData GetToolData() override;

private:

	MagnetismMode CurrentMode;




};

