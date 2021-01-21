#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
	bool Initialise();
	bool HandleKeyboard( MSG msg );
	void Update();
	void Draw();
private:

};

#endif