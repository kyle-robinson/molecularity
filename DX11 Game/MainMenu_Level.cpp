#include "stdafx.h"
#include "MainMenu_Level.h"
#include "Collisions.h"
#include "Rasterizer.h"


MainMenu_Level::MainMenu_Level(LevelStateMachine& stateMachine) : levelStateMachine(stateMachine){}

bool MainMenu_Level::OnCreate()
{
	try
	{
		// DRAWABLES
		{
			//add level UI 
			 Menu = make_shared<Main_Menu_UI>();
		
			 settingsUi = make_shared<Settings_Menu_UI>();
			

			
		}
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

void MainMenu_Level::OnSwitch()
{
	// update items on level switch here...
	levelName = "MainMenu";

	//make sure cursor is displayed
	EventSystem::Instance()->AddEvent(EVENTID::GamePauseEvent);

	//sounds
	Sound::Instance()->ClearAudio();

	Sound::Instance()->InitialiseMusicTrack( "Resources\\Audio\\Music\\MenuMusic.mp3", "MenuMusic" );
	Sound::Instance()->InitialiseSoundEffect( "Resources\\Audio\\Sounds\\Collision.mp3", "MenuClick" );

	//UI
	_UiManager->RemoveAllUI();
	_UiManager->AddUi(Menu, "MainMenu");
	_UiManager->AddUi(settingsUi, "Settings");
	_UiManager->Initialize(graphics->device.Get(), graphics->context.Get(), &cb_vs_matrix_2d);
	Sound::Instance()->PlayMusic( "MenuMusic" );
}

void MainMenu_Level::Render()
{
	// Render to sub viewport first using static camera
	GetMultiViewport()->SetUsingSub();
	BeginFrame();
	RenderFrame();

	// Render main scene next with main/debug camera
	GetMultiViewport()->SetUsingMain();
	BeginFrame();
	RenderFrame();

	// Render UI and present the complete frame
	EndFrame();
}
void MainMenu_Level::RenderFrame()
{
	// render ligths/skysphere
	LevelContainer::RenderFrameEarly();
	// render the cubes
	LevelContainer::RenderFrame();

	if (cameras->GetCurrentCamera() != JSON::CameraType::Static)
	{
		Shaders::BindShaders(graphics->context.Get(), graphics->vertexShader_2D, graphics->pixelShader_2D);
		cb_ps_scene.data.useTexture = TRUE;
		if (!cb_ps_scene.ApplyChanges()) return;
		graphics->context->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	}
}
void MainMenu_Level::Update(const float dt)
{
	// update lights/skysphere
	LevelContainer::Update(dt);

	// update cubes/multi-tool position
	LevelContainer::LateUpdate(dt);
}