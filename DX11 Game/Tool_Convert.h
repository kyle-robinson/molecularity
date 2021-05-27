#pragma once
#include "Tool_Function.h"

#include<Tool_Structs.h>


class Tool_Convert :
    public Tool_Function
{
public:
    Tool_Convert();
    ~Tool_Convert();

	 void ChangeCurrent(int number)override;
	 void ChangeCurrent(std::string name)override;
	 void  AddOneToCurrent() override;
	 void  MinusOneTOCurrent() override;
	 BoxType GetType() { return CurrentType; }

	 ToolData  GetToolData() override;
private:
	BoxType CurrentType;
};

