#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "Sprite.h"
//#include "Level.h"
#include "LevelStateMachine.h"

class Level1 : public Level
{
public:
	//bool Initialize( HWND hWnd, CameraController* camera, int width, int height );
	Level1( LevelStateMachine& stateMachine );
	bool OnCreate() override;
	void Render() override;
	void RenderFrame() override;
	void Update( const float dt ) override;
private:
	LevelStateMachine& levelStateMachine;

	// Scene Objects
	Sprite crosshair;
	RenderableGameObject hubRoom;
	RenderableGameObject skysphere;
	RenderableGameObject pressurePlate;

	// Textures
	std::unordered_map<BoxType, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> boxTextures;
};

#endif