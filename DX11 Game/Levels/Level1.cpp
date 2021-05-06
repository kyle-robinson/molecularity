#include "stdafx.h"
#include "Level1.h"
#include "Collisions.h"
#include "Rasterizer.h"

Level1::Level1( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) { }

bool Level1::OnCreate()
{
	try
	{
		// DRAWABLES
		{
			// models
			//if ( !hubRoom.Initialize( "Resources\\Models\\TestRoom.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			//if ( !hubRoom.Initialize( "Resources\\Models\\Flashlight.gbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );
			hubRoom.SetInitialPosition( 0.0f, 0.0f, 0.0f );

			if ( !pressurePlate.Initialize( "Resources\\Models\\PressurePlate.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			pressurePlate.SetInitialPosition( 0.0f, 0.0f, 15.0f );
			pressurePlate.SetInitialScale( 0.025f, 0.025f, 0.025f );
		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Level1::OnSwitch()
{
	// update items on level switch here...
}

void Level1::Render()
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

void Level1::RenderFrame()
{
	// render ligths/skysphere
	LevelContainer::RenderFrameEarly();

	// DRAWABLES
	{
		hubRoom.Draw();
		pressurePlate.Draw();

		// render objects (these are objects that are found in each level)
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
		}
	}
}

void Level1::Update( const float dt )
{
	LevelContainer::Update( dt );
	
	// camera world collisions. Will be player object collisions in the future and ideally not here
	if ( !Collisions::CheckCollisionCircle( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, 25.0f ) )
		cameras->CollisionResolution( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, dt );

	// update collisions w pressure plate
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		cubes[i]->CheckCollisionAABB( pressurePlate, dt );
		for ( uint32_t j = 0; j < NUM_CUBES; j++ )
			if ( i != j )
				cubes[i]->CheckCollisionAABB( cubes[j], dt );
	}

	LevelContainer::LateUpdate( dt );
}

void Level1::ProcessInput()
{
	LevelContainer::ProcessInput();

	// update level input here...
	// NOTE: not currently using
}