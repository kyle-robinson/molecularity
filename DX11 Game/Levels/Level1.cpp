#include "stdafx.h"
#include "Level1.h"
#include "Collisions.h"
#include "Rasterizer.h"
#include "StencilOutline.h"

Level1::Level1( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) { }

bool Level1::OnCreate()
{
	try
	{
		// DRAWABLES
		{
			// models
			if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );

			if ( !skysphere.Initialize( "Resources\\Models\\Sphere\\sphere.obj", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			skysphere.SetInitialScale( 250.0f, 250.0f, 250.0f );

			if ( !pressurePlate.Initialize( "Resources\\Models\\PressurePlate.fbx", graphics->device.Get(), graphics->context.Get(), cb_vs_matrix ) ) return false;
			pressurePlate.SetInitialPosition( 0.0f, 0.0f, 15.0f );
			pressurePlate.SetInitialScale( 0.025f, 0.025f, 0.025f );

			// sprites
			if ( !crosshair.Initialize( graphics->device.Get(), graphics->context.Get(), 16, 16, "Resources\\Textures\\crosshair.png", cb_vs_matrix_2d ) ) return false;
			crosshair.SetInitialPosition( graphics->GetWidth() / 2 - crosshair.GetWidth() / 2, graphics->GetHeight() / 2 - crosshair.GetHeight() / 2, 0 );
		}

		// TEXTURES
		{
			HRESULT hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\basic_crate.png", nullptr, boxTextures[BoxType::Default].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\bounce_crate.png", nullptr, boxTextures[BoxType::Bounce].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\arrow_crate.png", nullptr, boxTextures[BoxType::Arrow].GetAddressOf() );
			hr = CreateWICTextureFromFile( graphics->device.Get(), L"Resources\\Textures\\crates\\tnt_crate.png", nullptr, boxTextures[BoxType::TNT].GetAddressOf() );
			COM_ERROR_IF_FAILED( hr, "Failed to create texture from file!" );
		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Level1::Render()
{
	// Render to sub viewport first using static camera
	GetMultiViewport()->SetUsingSub( true );
	BeginFrame();
	RenderFrame();

	// Render main scene next with main/debug camera
	GetMultiViewport()->SetUsingMain( true );
	BeginFrame();
	RenderFrame();

	// Render UI and present the complete frame
	EndFrame();
}

void Level1::RenderFrame()
{
	// SKYSPHERE
	{
		Shaders::BindShaders( graphics->context.Get(), graphics->vertexShader_light, graphics->pixelShader_noLight );
		graphics->GetRasterizer( "Skybox" )->Bind( *graphics );
		skysphere.Draw();
		graphics->GetRasterizer( graphics->rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *graphics );
	}

	// LIGHTS
	{
		// w/out normals
		pointLight.Draw();
		directionalLight.Draw();

		// w/ normals
		graphics->context->PSSetShader( graphics->pixelShader_light.GetShader(), NULL, 0 );
		spotLight.Draw();
	}

	// DRAWABLES
	{
		hubRoom.Draw();
		pressurePlate.Draw();

		// w/ stencils
		for ( uint32_t i = 0; i < NUM_CUBES; i++ )
		{
			if ( cubes[i]->GetIsHovering() )
			{
				GetStencilOutline()->DrawWithOutline( *graphics, *cubes[i], cb_vs_matrix,
					pointLight.GetConstantBuffer(), boxTextures[cubes[i]->GetEditableProperties()->GetBoxType()].Get() );
			}
			else
			{
				cubes[i]->Draw( cb_vs_matrix, boxTextures[cubes[i]->GetEditableProperties()->GetBoxType()].Get() );
			}
		}
	}

	// SPRITES
	{
		if ( cameras->GetCurrentCamera() != JSON::CameraType::Static )
		{
			Shaders::BindShaders( graphics->context.Get(), graphics->vertexShader_2D, graphics->pixelShader_2D );
			cb_ps_scene.data.useTexture = TRUE;
			if ( !cb_ps_scene.ApplyChanges() ) return;
			graphics->context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
			crosshair.Draw( cameras->GetUICamera().GetWorldOrthoMatrix() );
		}
	}
}

void Level1::Update( const float dt )
{
	// update lights/skysphere
	pointLight.SetPosition( pointLight.GetLightPosition() );
	directionalLight.SetPosition( directionalLight.GetLightPosition() );
	skysphere.SetPosition( cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionFloat3() );

	// camera world collisions. Will be player object collisions in the future and ideally not here
	if ( !Collisions::CheckCollisionCircle( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, 25.0f ) )
		cameras->CollisionResolution( cameras->GetCamera( JSON::CameraType::Default ), hubRoom, dt );

	// update cubes
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		// update cube scale multiplier
		if ( cubes[i]->GetEditableProperties()->GetToolType() == ToolType::Resize )
			cubes[i]->SetScale( static_cast<float>( cubes[i]->GetEditableProperties()->GetSizeMultiplier() ) );

		// cube range collision check
		cubes[i]->SetIsInRange( Collisions::CheckCollisionSphere( cameras->GetCamera( cameras->GetCurrentCamera() ), *cubes[i], 5.0f ) );

		// update objects
		cubes[i]->Update( dt );
	}

	// update collisions
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		cubes[i]->CheckCollisionAABB( pressurePlate, dt );
		for ( uint32_t j = 0; j < NUM_CUBES; j++ )
			if ( i != j )
				cubes[i]->CheckCollisionAABB( cubes[j], dt );
	}

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras->GetCamera( JSON::CameraType::Default ) );
}