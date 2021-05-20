#pragma once
#include "Tool_Function.h"
#include<Tool_Structs.h>

class Tool_Resize :
    public Tool_Function
{

public:
	Tool_Resize();
	~Tool_Resize();

	void ChangeCurrent(int number)override;
	void ChangeCurrent(std::string name)override;
	void  addoneTOCurrent() override;
	void  minusoneTOCurrent() override;
	BoxSize GetSize() { return CurrentSize; }
	ToolData GetToolData() override;
private:
	BoxSize CurrentSize;
};

