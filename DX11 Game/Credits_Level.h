#pragma once
#include "LevelContainer.h"

#include "LevelStateMachine.h"

class Credits_Level :
    public LevelContainer
{
public:
	Credits_Level(LevelStateMachine& stateMachine);
	bool OnCreate() override;
	void OnSwitch() override;
	void Render() override;
	void Update(const float dt) override;
	void ProcessInput() override;
private:
	void RenderFrame() override;
	LevelStateMachine& levelStateMachine;





};

