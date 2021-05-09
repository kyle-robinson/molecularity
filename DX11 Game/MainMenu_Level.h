#pragma once
#include "LevelStateMachine.h"
class MainMenu_Level :
    public LevelContainer
{
public:
	MainMenu_Level(LevelStateMachine& stateMachine);
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update(const float dt) override;
	void ProcessInput() override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;

};

