#include "stdafx.h"
#include "Level2.h"
#include "Collisions.h"
#include "Rasterizer.h"
//ui
#include<Graphics/UI_Manager.h>
#include<UI/HUD_UI.h>
#include<UI/Pause.h>
#include<UI/Settings_Menu_UI.h>

Level2::Level2( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool Level2::OnCreate()
{
	try
	{
		// DRAWABLES
		{
			// models
			if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );

			// sprites
			if ( !crosshair.Initialize( graphics->device.Get(), graphics->context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
			crosshair.SetInitialPosition( graphics->GetWidth() / 2 - crosshair.GetWidth() / 2, graphics->GetHeight() / 2 - crosshair.GetHeight() / 2, 0 );

		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Level2::OnSwitch()
{
	// update items on level switch here...
	levelName = "Level2";

	Sound::Instance()->ClearAudio();

	Sound::Instance()->InitialiseMusicTrack( "Resources\\Audio\\Music\\LevelMusic.mp3", "LevelMusic" );
	Sound::Instance()->InitialiseSoundEffect( "Resources\\Audio\\Sounds\\ToolUse.mp3", "ToolUse" );
	Sound::Instance()->InitialiseSoundEffect( "Resources\\Audio\\Sounds\\MenuClick.mp3", "MenuClick" );

	Sound::Instance()->PlayMusic( "LevelMusic" );
}

void Level2::Render()
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

void Level2::RenderFrame()
{
	// render ligths/skysphere
	LevelContainer::RenderFrameEarly();

	// DRAWABLES
	{
		hubRoom.Draw();

		// render the cubes
		LevelContainer::RenderFrame();
	}

	// SPRITES
	{
		if ( cameras->GetCurrentCamera() != JSON::CameraType::Static )
		{
			Shaders::BindShaders( graphics->context.Get(), graphics->vertexShader_2D, graphics->pixelShader_2D );
			cb_ps_scene.data.useTexture = TRUE;
			if ( !cb_ps_scene.ApplyChanges() ) return;
			graphics->context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
			//crosshair.Draw( cameras->GetUICamera().GetWorldOrthoMatrix() );
		}
	}
}

void Level2::Update( const float dt )
{
	// update lights/skysphere
	LevelContainer::Update( dt );

	// camera world collisions. Will be player object collisions in the future and ideally not here
	if ( !Collisions::CheckCollisionCircle( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, 25.0f ) )
		cameras->CollisionResolution( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, dt );

	// update cubes/multi-tool position
	LevelContainer::LateUpdate( dt );
}