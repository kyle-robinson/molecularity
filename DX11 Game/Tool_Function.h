#pragma once

#include<Tool_Structs.h>

class Tool_Function
{
public:
	Tool_Function() {}
	~Tool_Function() {}

	void Update();

	void virtual ChangeCurrent(int number) = 0;
	void virtual ChangeCurrent(std::string name) = 0;
	void virtual addoneTOCurrent() = 0;
	void virtual minusoneTOCurrent() = 0;
	ToolData virtual GetToolData() { return ToolData(); };


protected:
	 int current = 0;
};

