#pragma once
#include "Tool_Function.h"
class Tool_Conductivity :
    public Tool_Function
{
public:
	Tool_Conductivity();
	~Tool_Conductivity();
	void ChangeCurrent(int number)override;
	void ChangeCurrent(std::string name)override;
	void AddOneToCurrent() override;
	void MinusOneTOCurrent() override;

	BoxConductive GetType() { return CurrentConductive; }
	ToolData  GetToolData() override;

private:

	BoxConductive CurrentConductive;


};

